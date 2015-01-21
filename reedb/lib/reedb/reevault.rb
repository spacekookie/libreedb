# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# => Import custom errors
require_relative 'errors/encryption_errors'
require_relative 'errors/vault_errors'

# => Import utilities and tools
require_relative 'utils/utilities'
require_relative 'utils/logger'

# => Import security package classes
require_relative 'security/multifish'
require_relative 'security/twofish'
require_relative 'security/aes'

# => Import internals
require 'fileutils'
require 'socket'
require 'yaml' # Used for configuration files.
require 'bson' # Used for data files.

module Reedb
	class ReeVault

		# Constructor for a vault with name, path and encryption enum.
		# Valid encryption parameters are :aes, :twofish, :multi and :auto_fill
		#
		def initialize(name, path, encprytion)
			@already_logging = false
			construct_path("#{name}", "#{path}")
			init_encryption(encprytion) # => Throws exceptions!
			self.secure_config(false)
			return self
		end

		def secure_config(boolean = true)
			@secure_config = boolean
			return self
		end

		def create(password)
			return nil unless password?(password)
			return nil unless encryption?(password)
			
			# => Encryption now active and key available under @crypt.key

			conf_path = Reedb::Utilities::append_to_path("#{@path}", 'config')

			begin
				needs_creation = true

				if self.includes?('config')
					raise VaultExistsAtLocationError.new, "Vault already exists at location #{@path}.\nLoading existing vault instead...\n"
					
					# => This rules out lots of code to be run
					needs_creation = false
				else
					if Reedb::archos == :unix
						FileUtils::mkdir_p("#{@path}/data") # => Data dir
						FileUtils::mkdir("#{@path}/shasums") # => Checksum dir
						FileUtils::mkdir("#{@path}/logs") # => Logs dir

						FileUtils::chmod_R(0744, "#{@path}")
					else
						FileUtils::mkdir_p("#{@path}\\data") # => Data dir
						FileUtils::mkdir("#{@path}\\shasums") # => Checksum dir
						FileUtils::mkdir("#{@path}\\logs") # => Logs dir
					end
				end

				# Now that the vault directory exists logs can be opened.
				init_logger(true)

				if needs_creation
					# Code that will only be run if the vault was just created on the system
					time = Reedb::Utilities.get_time(false)
					VaultLogger.write("Vault created on #{time}. All directories created successfully!")

					# => Now creating configuration file
					config = {}
					config['vault_name'] = "#{@name}"
					config['creation_date'] = "#{Utilities.get_time}"
					config['last_updated'] = "#{Utilities.get_time}"
					config['creation_machine'] = "#{Socket.gethostname}"
					config['updating_machine'] = "#{Socket.gethostname}"
					config['creation_user'] = "nil" # => Think of clever way to do this
					config['updating_user'] = "nil"

					hashed_pw = SecurityUtils::tiger_hash("#{password}") #TODO: Decide what to do with this.

					# => Writing configuration to disk. Either securely or insecurely
					if @secure_config
						update_secure_info('config', config)
						par_path = Reedb::Utilities::append_to_path("#{@path}", "pom")
						msg = "Why are you reading this?"
						File.open("#{par_path}", "wb").write(Base64.encode64("#{msg}"))
					else
						File.open("#{conf_path}", "w"){ |f| YAML.dump(config, f) }
					end

					# Now writing encrypted key to file with ASCII armour
					update_secure_info("cey", @encrypted_key)
					remove_instance_variable(:@encrypted_key)
				end

			# All other errors won't be caught!
			rescue EncryptionError, VaultError => e
				puts e.message
			end

			self.load("#{password}")
		end

		def load(password)
			self.includes?('config') # => Returns the existance of the central config

			init_logger(false)

			# Check if the config needs to be read via ASCII64 or YAML
			if self.includes?('pom')
				# Config is stored with ASCII Armour
				@config = read_secure_info('config')
			else
				conp = Reedb::Utilities::append_to_path(@path, 'config')
				@config = YAML.load_file(conp)
			end
			puts @config

			return self

			unlock_vault("#{password}") # => Reading config from file

			@header = {}

			# => Cache the vault
			cache_header
			cache_files
			return self

		end

		def close
			@crypt.stop_encryption if @crypt.init
		end

		# Quickly returns if a file exists in the vault or it's children.
		def includes?(file)
			file_to_check = Reedb::Utilities::append_to_path("#{@path}", "#{file}")
			return File.exists?("#{file_to_check}")
		end

		private

		def cache_header
			Dir.glob("#{@path}/data/*.ree") do |file|
				f = File.open(file, 'r')

				encrypted = f.read
				decrypted = @krypt.decrypt_string(encrypted)
				yaml = YAML.load(decrypted)

				@header["#{Pathname.new("#{file}").basename}"] = {}
				@header["#{Pathname.new("#{file}").basename}"]['name'] = yaml['header']['name']
				@header["#{Pathname.new("#{file}").basename}"]['url'] = yaml['header']['url']
				@header["#{Pathname.new("#{file}").basename}"]['category'] = yaml['header']['category']
				@header["#{Pathname.new("#{file}").basename}"]['latest'] = yaml['header']['latest']	
			end
		end


		def cache_header(mode = :secure)
			@header = {}
			VaultLogger.write("Starting a cache cycle at #{Reedb::Utilities::get_time} in #{mode} mode.", 'debug')

			Dir.glob(Reedb::Utilities::append_to_path_dir(@path, ['data', '*.ree'])) do |file|

				f = File.open(file, 'r')
				encryted = f.read
				decrypted = @crypt.decrypt_string(encrypted)

			end
			if mode == :fast
				puts "This isn't implemented yet and you should really stick to secure mode!"
			end


		end

		def cache_files(mode = :secure)

		end

		# Builds the vault path from a path, name and trimming
		# additional slashes from the end.
		#
		def construct_path(name, path)
			(@name = name ; @path = "")
			if Reedb::archos == :unix
				path.end_with?("/") ? @path = "#{path}#{name}.reevault" : @path = "#{path}/#{name}.reevault"
			else
				path.end_with?("\\") ? @path = "#{path}#{name}.reevault" : @path = "#{path}\\#{name}.reevault"
			end
		end

		def update_secure_info(name, data = nil)
			path = Reedb::Utilities::append_to_path(@path, "#{name}")
			File.open(path, "wb").write(Base64.encode64("#{data}"))
		end

		def read_secure_info(name)
			path = Reedb::Utilities::append_to_path(@path, "#{name}")
			return Base64.decode64(File.open(path, "r").read())
		end

		def init_logger(bol)
			begin
				unless logger?(bol)
					raise VaultLoggerError.new, "Logger failed to be initialised" 
				end
			rescue VaultError => e
				puts e.message
			end
		end

		def logger?(bol)
			(return false) if @already_logging && bol

			VaultLogger.setup("#{@path}")
			(@already_logging = true ; return true)
		end

		def password?(password)
			begin
				raise MissingUserPasswordError.new, "Encryption error: Missing user password!" if password == nil

				raise InsecureUserPasswordError.new, "Encryption error: Password too short!" if password.length < Reedb::passlength
			rescue EncryptionError => e
			 	puts e.message
			 	return false
			end
			return true
		end

		def encryption?(password)
			begin
				@encrypted_key = @crypt.start_encryption(password)
			rescue EncryptionError => e
				puts e.message
				return false
			end
			return true
		end

		# This method checks what encryption to use by enums.
		# This can throw an exception if something was parsed incorrectly
		# After this call the @crypt object has been initialised.
		#
		def init_encryption(type)
			type = :aes if type == :auto_fill
			begin
				if type == :aes
					@crypt = Reedb::RAES.new
				elsif type == :twofish
					@crypt = Reedb::Fish.new
				elsif type == :multi
					@crypt = Reedb::MLE.new
				else
					raise MissingEncryptionTypeError.new, "Encryption failed: Missing type. Aborting..."
				end
			rescue EncryptionError => e
			 	puts e.message
			end
		end
	end
end