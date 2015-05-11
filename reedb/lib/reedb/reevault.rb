# ====================================================
# Copyright 2015 Lonely Robot (see @author)
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
require_relative 'utils/sorting'
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

		# Indicates the size of the vault as per data file entries.
		# Is updated with every header cache and write cycle.
		#
		attr_reader :size

		# Holds a hash of possible values that header files in this vault
		# can have. Fields need to be specified by name and a type. To choose
		# from 'single', 'list' and 'tree' (var, list, dict)
		#
		attr_reader :header_set

		# Constructor for a vault with name, path and encryption enum.
		# Valid encryption parameters are :aes, :twofish, :multi and :auto_fill
		#
		def initialize(name, path, encprytion)
			@already_logging = false
			
			# Header maps
			@headers = {}
			@hgroups = {}

			# Fileset
			@locked = false
			@locks = []

			# Defines the default (and boring vanilla) header set
			@header_set = {
				'urls'=>'list',
				'tags'=>'list'
			}

			construct_path("#{name}", "#{path}")
			init_encryption(encprytion) # => Throws exceptions!
			self.secure_config(false)
			return self
		end

		def secure_config(boolean = true)
			@secure_config = boolean
			return self
		end

		# Fields can be added to a vault header BUT NOT REMOVED AGAIN!
		# So be careful what you put in your header.
		# (aka upgrade yes, downgrade noooo)
		#
		# Unused fields can remain blank but need to stay in a vaults header list
		# for backwards compatiblity
		#
		def add_header_field(name, type)
			@header_set[name] = type unless @header_set[name]
		end


		# Pokes if a vault exists
		def try?
			return self.includes?('config')
		end

		# Counts the vault contents and returns an Integer
		# WITHOUT HAVING TO UNLOCK THE VAULT!
		#
		def count
			counter = 0
			Dir.glob("#{@path}/data/*.ree") do |f|
				counter += 1
			end
			return counter
		end

		# Little helper method to determine if a vault is in the middle of 
		# a write cycle. Which would cause horrible crashes on other applications
		# and errors on the file system if things are moved around inside
		#
		def locked?() return @locked end

		def create(password = :failed)
			# If keygen was used to set a user password then fetch it
			# and remove the variable from memory!
			return nil unless password?(password)
			return nil unless encryption?(password)

			# puts "This is the password: #{password}"
			
			# => Encryption now active and key available under @crypt.key
			@conf_path = "#{@path}/config"

			needs_creation = true

			if self.includes?('config')
				raise VaultExistsAtLocationError.new, "Vault already exists at location #{@path}.\nLoading existing vault instead...\n"
				
				# => This rules out lots of code to be run
				needs_creation = false
			else
				if Reedb::archos == :linux || Reedb::archos == :osx
					FileUtils::mkdir_p(File.expand_path("#{@path}/data")) # => Data dir
					FileUtils::mkdir(File.expand_path("#{@path}/shasums")) # => Checksum dir
					FileUtils::mkdir(File.expand_path("#{@path}/logs")) # => Logs dir

					FileUtils::chmod_R(0744, "#{@path}")

				# This is used for windows because windows fucking sucks!
				else
					FileUtils::mkdir_p(File.expand_path("#{@path}/data")) # => Data dir
					FileUtils::mkdir(File.expand_path("#{@path}/shasums")) # => Checksum dir
					FileUtils::mkdir(File.expand_path("#{@path}/logs")) # => Logs dir
				end
			end

			# Now that the vault directory exists logs can be opened.
			init_logger(true)

			if needs_creation
				# Code that will only be run if the vault was just created on the system
				time = Reedb::Utilities.get_time(false)
				VaultLogger.write("Vault created on #{time}. All directories created successfully!")

				# => Now creating configuration file
				@config = {}
				@config['vault_name'] = "#{@name}"
				@config['creation_date'] = "#{Utilities.get_time}"
				@config['last_updated'] = "#{Utilities.get_time}"
				@config['creation_machine'] = "#{Socket.gethostname}"
				@config['updating_machine'] = "#{Socket.gethostname}"
				@config['creation_user'] = "#{Etc.getlogin}"
				@config['updating_user'] = "#{Etc.getlogin}"
				@config['header_set'] = "#{@header_set}"
				save_config
				
				# Now writing encrypted key to file with ASCII armour
				update_secure_info("cey", @encrypted_key)
				# remove_instance_variable(:@encrypted_key)
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

			# @header_set = @config['header_set']

			return nil unless unlock_vault("#{password}")
			VaultLogger.write("Finished loading vault", 'debug')
			cache_headers

			return self
		end

		# Read a single file from the vault in secure mode
		# Returns the entire file or only it's current set in hashes.
		#
		def read_file(name, history = false)

			# Loads the file data into a local variable if it exists
			file_data = load_file_data(name) 
			if file_data == nil
				raise FileNotFoundError.new("#{name} could not be read: File not found!")
				return VAULT_FILE_NOT_FOUND_ERROR # If the exception isn't handled correctly
			else
				# This code is executed if the file was found (thus data is in file_data)
				compiled = {}
				compiled['header'] = {}

				# Removes the latest version from the header because it is insignificant.
				file_data['header'].each do |key, value|
					compiled['header']["#{key}"] = value unless key == "latest"
				end

				if history
					compiled['body'] = file_data['body']
				else
					body_list = []
					file_data['body'].each do |key, value|
						body_list << key
					end

					compiled['body'] = {}

					# Now sort the list of body versions
					body_list.heapsort!
					body_list.each do |version|
						file_data['body']["#{version}"].each do |key, value|
							compiled['body']["#{key}"] = value
						end
					end
				end
				
				# Then return that value
				return compiled
			end
		end

		# Check the file API or the wiki to learn how this function works.
		# This function is also used to delete fields from header space.
		#
		def update(name, data)
			cache_headers # Cache headers first to be sure

			(raise FileBusyError.new, "File #{name} busy" ; return) if @locks.include?(name)
			@locks << name

			if @headers.key?(name)
				# Creates file object from existing file object.
				df = DataFile.new(name, self, load_file_data(name, :secure))
				df.insertv2(data, :hard) # Default cache mode
			else
				df = DataFile.new(name, self)
				df.insertv2(data, :hard) # Default cache mode
			end

			# => Make sure that everything is up to date.
			cache_headers
			@config['updating_user'] = "#{Etc.getlogin}"
			@config['updating_machine'] = "#{Socket.gethostname}"
			@config['last_updated'] = "#{Utilities.get_time}"
			save_config

			# Sync and close the file.
			df.sync.close

			# Unlocks the file again for other processes to edit.
			@locks.delete(name)
		end

		def remove_file name
			path_to_file = load_file_hash(name)
			if path_to_file
				FileUtils.rm(path_to_file)
				puts "Successfullly removed file #{name}"
				VaultLogger.write("Removed file #{name} from vault.", 'debug')
			else
				raise FileNotFoundError.new("#{name} could not be removed: File not found!")
			end
		end

		# Returns headers according to a search queury
		#
		# { 'name' => '__name__',
		# 	'url' => '__url__',
		# 	'tags' => '__tags__',
		# 	'generic_field' => '__generic_information__'
		# }
		#
		def list_headers search
			cache_headers
			return @headers
		end

		# Dump headers and files from memory in times of
		# inactivity for security reasons
		def unload
			remove_instance_variable(:@headers)
			@headers = {}

			VaultLogger.write("It has been $TIME since the last interaction. Unloading vault contents for security reasons.", 'debug')
		end

		def close
			VaultLogger.write("Scheduled closing of the vault.", 'debug')
			@crypt.stop_encryption if @crypt.init

			# Removing class variables for cleanup
			remove_instance_variable(:@crypt)
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

		# Caches the current set of headers on a vault.
		# 
		def cache_headers
			@headers = {}


			VaultLogger.write("Starting a cache cycle.", 'debug')

			Dir.glob("#{@path}/data/*.ree") do |file|
				f = File.open(file, 'r')
				encrypted = Base64.decode64(f.read)
				decrypted = @crypt.decrypt(encrypted)

				data = JSON.parse(decrypted)
				df = DataFile.new(nil, self, data)

				tmp_head = df.cache(:header)
				tmp_name = df.name

				# Blank the df variable just in case.
				df = 0x111111111111111
				
				@headers[tmp_name] = tmp_head 
				
				# Now work with the header set to determine sub-groups
				tmp_head.each do |category, data|
					if @header_set.include?(category)
						# This will loop through all the category groups in the
						# header that have been registered

						# Creates a new sub-hash for data
						@hgroups["#{category}"] = {} unless @hgroups["#{category}"]

						# Create a list to represent files that match the information
						@hgroups["#{category}"]["#{data}"] = [] unless @hgroups["#{category}"]["#{data}"]

						# Now go through the data per data type and sort it into
						# the apropriate category section.
						#
						if @header_set["#{category}"] == 'single'
							@hgroups["#{category}"]["#{data}"] << data

						# If inserting into a header list
						elsif @header_set["#{category}"] == 'list'
							data.each do |value|
								@hgroups["#{category}"]["#{data}"] << value
							end
						# If inserting into a header tree
						elsif @header_set["#{category}"] == 'tree'
							@hgroups["#{category}"]["#{data}"] << data
						end
					end
				end
			end
		end

		def load_file_hash name
			cache_headers
			if @headers.key?(name)
				name_hash = SecurityUtils::tiger_hash("#{name}")
				return "#{@path}/data/#{name_hash}.ree"
			else
				return false
			end
		end

		# Loads a file with the clear name from headers.
		# If file isn't found in headers vault is recached
		# and file is then loaded from headers.
		#
		# If file isn't found in headers error is output.
		#
		def load_file_data(name, mode = :secure)
			cache_headers
			VaultLogger.write("Loading file #{name} from vault", 'debug')
			if @headers.key?(name)
				name_hash = SecurityUtils::tiger_hash("#{name}")
				file_path = "#{@path}/data/#{name_hash}.ree"
				f = File.open(file_path, 'r')
				encrypted = Base64.decode64(f.read)
				decrypted = @crypt.decrypt(encrypted)

				return JSON.parse(decrypted) if mode == :secure
			else
				return nil
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

		# Unlocks the vault by decrypting the key and loading it into memory
		# Enables the cryptographic module to decrypt and encrypt files.
		#
		def unlock_vault pw
			@encrypted_key = read_secure_info('cey') unless @encrypted_key
			@crypt.start_encryption(pw, @encrypted_key)
			remove_instance_variable(:@encrypted_key) if @encrypted_key
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
			type = :aes if type == :auto
			if type == :aes
				@crypt = Reedb::RAES.new
			elsif type == :twofish
				@crypt = Reedb::Fish.new
			elsif type == :multi
				@crypt = Reedb::MLE.new
			else
				raise MissingEncryptionTypeError.new, "Encryption failed: Missing type. Aborting..."
			end
		end
	
	end # class close
end # module close
