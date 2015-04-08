# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# => Vault internals
require_relative 'datafile'

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
require 'json'
require 'yaml'
require 'etc'

module Reedb
	class ReeVault

		attr_reader :path

		# Encryption handler to be used by the vault files
		#
		attr_reader :crypt

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

		def keygen(password)
			@__PW__ = password
		end

		def create(password = :failed)
			# If keygen was used to set a user password then fetch it
			# and remove the variable from memory!
			(password = @__PW__ ; remove_instance_variable(:@__PW__)) if password == :failed
			return nil unless password?(password)
			return nil unless encryption?(password)
			
			# => Encryption now active and key available under @crypt.key

			@conf_path = "#{@path}/config"

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
					config['creation_user'] = "#{Etc.getlogin}"
					config['updating_user'] = "#{Etc.getlogin}"

					# hashed_pw = SecurityUtils::tiger_hash("#{password}") #TODO: Decide what to do with this.

					# => Writing configuration to disk. Either securely or insecurely
					save_config

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
			unless self.includes?('config')
				raise VaultDoesNotExistError.new("Loading vault failed because it couldn't be found at the specified path!")
			end
			init_logger(false)

			# Check if the config needs to be read via ASCII64 or YAML
			if self.includes?('pom')
				# Config is stored with ASCII Armour
				@config = read_secure_info('config')
			else
				@config = YAML.load_file("#{@path}/config")
			end

			@headers = {}
			@files = {}

			return nil unless unlock_vault("#{password}")
			puts "Just finished loading..."
			cache_headers

			return self
		end

		# Read a single file from the vault in secure mode
		# Returns the entire file in hashes (so includes all sub-versions)
		#
		def read_file name
			tmp = load_file(name, :secure)
			unless tmp
				puts "File not found!"
			else
				return load_file(name, :secure)
			end
		end

		# Inserts into a file of the name that's specified in the function.
		# Data is stored in nested hashes:
		#
		# {:headers=>{'name'=>'your_name_here'}, :body=>{'password'=>'secure_password'}}
		#
		#
		def insert(name, data)
			if @headers.key?(name)
				VaultLogger.write("File #{name} already exists. Going to edit mode.", 'debug')
				
				unless load_file(name, :fast)
					VaultLogger.write("Discrepency between two header cache cycles detected! Aborting insertion. Output warning to user!", 'fatal')
					raise BadCacheError.new, "Insertion failed because of an invalid cache. Check your filesystem and try again!"
					return nil
				end
				df = DataFile.new(name, self, @files[name])
				df.insert(data)
			else
				df = DataFile.new(name, self)
				df.insert(data)
			end
			# => Make sure that everything is up to date.
			cache_headers
			@config['updating_user'] = "#{Etc.getlogin}"
			@config['updating_machine'] = "#{Socket.gethostname}"
			@config['last_updated'] = "#{Utilities.get_time}"
			save_config

			# Check for vault sync option here
			# and then sync in an asynchronous thread or something...
		end

		# Dump headers and files from memory in times of
		# inactivity for security reasons
		def unload
			remove_instance_variable(:@files)
			@files = {}
			remove_instance_variable(:@headers)
			@headers = {}

			VaultLogger.write("It has been $TIME since the last interaction. Unloading vault contents for security reasons.", 'debug')
		end

		def close
			VaultLogger.write("Scheduled closing of the vault.", 'debug')
			@crypt.stop_encryption if @crypt.init

			# Removing class variables for cleanup
			remove_instance_variable(:@crypt)
			remove_instance_variable(:@files)
			remove_instance_variable(:@headers)
		end

		# Quickly returns if a file exists in the vault or it's children.
		def includes? file
			return File.exists?("#{@path}/#{file}")
		end

		def to_s
			return "Vault: #{@name}, Path: #{@path}, File count: #{@headers.length}"
		end

		private

		# Caches the file contents of the vault for every file.
		# This is considered insecure!
		# def cache_files(mode = :default)
		# 	@files = {}
		# 	VaultLogger.write("Starting a full file cache cycle at #{Reedb::Utilities::get_time} in #{mode} mode.", 'debug')

		# 	Dir.glob("#{@path}/data/*.ree") do |file|
		# 		f = File.open(file, 'r')
		# 		encrypted = Base64.decode64(f.read)
		# 		decrypted = @crypt.decrypt(encrypted)

		# 		data = JSON.parse(decrypted)
		# 		df = DataFile.new(nil, self, data)
		# 		@files[df.name] = df.cache(:full)
		# 	end
		# end

		def cache_headers
			@headers = {}
			VaultLogger.write("Starting a cache cycle at #{Reedb::Utilities::get_time}.", 'debug')

			Dir.glob("#{@path}/data/*.ree") do |file|
				f = File.open(file, 'r')
				encrypted = Base64.decode64(f.read)
				decrypted = @crypt.decrypt(encrypted)

				data = JSON.parse(decrypted)
				df = DataFile.new(nil, self, data)
				@headers[df.name] = df.cache(:header)
			end
		end

		# Loads a file with the clear name from headers.
		# If file isn't found in headers vault is recached
		# and file is then loaded from headers.
		#
		# If file isn't found in headers error is output.
		#
		def load_file(name, mode = :secure)
			cache_headers
			VaultLogger.write("Loading file #{name} from vault", 'debug')
			if @headers.key?(name)
				name_hash = SecurityUtils::tiger_hash("#{name}")
				file_path = "#{@path}/data/#{name_hash}.ree"
				f = File.open(file_path, 'r')
				encrypted = Base64.decode64(f.read)
				decrypted = @crypt.decrypt(encrypted)

				data = JSON.parse(decrypted)

				df = DataFile.new(nil, self, data)
				@files[df.name] = df.cache(:full)

				if mode == :secure
					tmp = @files[name]
					df.close 
					@files[name] = nil
					return tmp
				end
				return true
			else
				return false
			end
		end

		def save_config
			@conf_path = "#{@path}/config"
			if @secure_config
				update_secure_info('config', @config)
				par_path = "#{@path}/pom"
				msg = "Why are you reading this?"
				File.open("#{par_path}", "wb").write(Base64.encode64("#{msg}"))
			else
				File.open("#{@conf_path}", "wb+"){ |f| YAML.dump(@config, f) }
			end
		end

		# Builds the vault path from a path, name and trimming
		# additional slashes from the end.
		#
		def construct_path(name, path)
			(@name = name ; @path = "")
			path.end_with?("/") ? @path = "#{path}#{name}.reevault" : @path = "#{path}/#{name}.reevault"
		end

		def update_secure_info(name, data = nil)
			path = "#{@path}/#{name}"
			File.open(path, "wb").write(Base64.encode64("#{data}"))
		end

		def read_secure_info name
			path = "#{@path}/#{name}"
			return Base64.decode64(File.open(path, "r").read())
		end

		def init_logger bool
			begin
				unless logger?(bool)
					raise VaultLoggerError.new, "Logger failed to be initialised" 
				end
			rescue VaultError => e
				puts e.message
			end
		end

		def logger? bool
			(return false) if @already_logging && bool

			VaultLogger.setup("#{@path}")
			(@already_logging = true ; return true)
		end

		def password? password
			begin
				raise MissingUserPasswordError.new, "Encryption error: Missing user password!" if password == nil

				raise InsecureUserPasswordError.new, "Encryption error: Password too short!" if password.length < Reedb::passlength
			rescue EncryptionError => e
			 	puts e.message
			 	return false
			end
			return true
		end

		def encryption? password
			begin
				@encrypted_key = @crypt.start_encryption(password)
			rescue EncryptionError => e
				puts e.message
				return false
			end
			return true
		end

		def unlock_vault pw
			@encrypted_key = read_secure_info('cey') unless @encrypted_key
			@crypt.start_encryption(pw, @encrypted_key)
			remove_instance_variable(:@encrypted_key)
			return true if @crypt.init

			# If encryption init failed...
			puts "Wrong user password!"
			return false
		end

		# This method checks what encryption to use by enums.
		# This can throw an exception if something was parsed incorrectly
		# After this call the @crypt object has been initialised.
		#
		def init_encryption type
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
	
	end # class close
end # module close