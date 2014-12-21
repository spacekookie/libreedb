#!/usr/bin/ruby


# @license: MIT License
# @author: Katharina Sabel

# => Internal Requires
require_relative 'logger/vault_logger'
require_relative 'security/robocrypt'
require_relative 'logger/app_logger'
require_relative 'utilities/general'
require_relative 'utilities/time'
require_relative 'data_file'

# => External Requires
require 'digest/sha1'
require 'fileutils'
require 'pathname'
require 'socket'
require 'yaml'

module Reedb

	class ReeVault

		# The vault config located in the vaults root directory. Is updated after every write cycle to
		# write new information about last-updated and update machine. Contains encrypted key and hashed password.
		#
		attr_reader :config

		# The headers of the vault files that are cached after every write cycle. To be given or taken by extentions
		# to build external file views.
		#
		attr_reader :header

		# A hash with all files included in the vault, with their nested values. During runtime this contains the entire
		# stack of files from the vault.
		#
		# TODO: Remove because of obvious memory leaking issues!
		#
		attr_reader :files

		# The path to the vault on the disk used for file reads and creations.
		#
		attr_reader :path

		# The name of the vault to be built into the path and used for debugging and user-experience purposes.
		#
		attr_reader :name

		# An attached crypto-class from the RoboCrypt module. Handles all encrytion externally.
		#
		attr_reader :krypt

		# Initialises the vault, sets an empty path and registeres a new crypto-class onto the
		#
		def initialize(crypto="aes")
			@path = ""
			@already_logging = false

			# => TODO: Determine what encryption module to use
			(@krypt = Robocrypt::RoboAES.new) if crypto == "aes"
			return self
		end

		def init_logger
			VaultLogger.setup("#{@path}") unless @already_logging
			@already_logging = true
		end

		def create(name, path, password)
			(puts "PASSWORD CAN'T BE NIL!" ; return) if password == nil
			@name = name
			construct_path(name, path)

			if File.exists?("#{@path}/global.yml")
				puts "VAULT ALREADY EXISTS: PLEASE CHANGE NAME OR CHOOSE A DIFFERENT LOCATION!"
				init_logger
				VaultLogger.write("Vault already exists. Not overwriting!")

				# TODO: Possibly ask to load the vault instead?
				# TODO: Stop shouting at the user :)
				return
			end

			FileUtils::mkdir_p("#{@path}/data") # => Data dir
			FileUtils::mkdir("#{@path}/shasums") # => Checksum dir
			FileUtils::mkdir("#{@path}/logs") # => Logs dir

			FileUtils::chmod_R(0744, "#{@path}") #TODO: Check

			# Now that folder exists => Start logs
			init_logger

			VaultLogger.write("Vault created on #{Reepass::Utilities.get_time(false)}. All directories created successfully!")

			encrypted_key = @krypt.init_encryption("#{password}")
			hashed_pw = Robocrypt::CryptUtils.hash_base512("#{password}")
			
			config = {}
			config['vault_name'] = "#{name}"
			config['creation_date'] = "#{ReeTime.get_time}"
			config['last_updated'] = "#{ReeTime.get_time}"
			config['khash'] = "#{encrypted_key}"
			config['phash'] = "#{hashed_pw}"
			config['creation_machine'] = "#{Socket.gethostname}"
			config['updating_machine'] = "#{Socket.gethostname}"

			ReeLogger.write("Wrote key and pw hashes to config files", "info")
			config['locked'] = false

			File.open("#{@path}/global.yml", "w"){ |file| YAML.dump(config, file) }

			# => Load the vault like it existed before!
			load(name, path, "#{password}",false)
		end

		def load(name, path, password, read_only = false)
			construct_path(name, path)
			unless Dir.exists?("#{@path}")
				puts "Error: Vault couldn't be found!"
				ReeLogger.write("Vault couldn't be found from app config. Is it corrupt? ", "fatal")
				exit
			end

			init_logger
			@read_only = read_only
			
			unlock_vault("#{password}") # => Reading config from file

			@header = {}

			# => Cache the vault
			cache_header
			cache_files
			return self
		end

		def construct_path(name, path)
			@name = name
			path.end_with?("/") ? @path = "#{path}#{name}.reevault" : @path = "#{path}/#{name}.reevault"
		end

		def cache_header
			@header = {}
			VaultLogger.write("Caching headers!", 'debug')
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

		def unlock_vault(password)
			@config = YAML.load_file("#{@path}/global.yml")
			# TODO: Check if the vault name has been changed, log the change and write it to the configuration file
			# => Setup crypt-component
			unless @krypt.init
				pw_hash = @config['phash']
				key_hash = @config['khash']

				input_hash = Robocrypt::CryptUtils.hash_base512("#{password}")

				@krypt.init_encryption("#{password}", "#{key_hash}") if pw_hash == input_hash
				(puts "Authentication failed: Wrong password!" ; exit ) if pw_hash != input_hash
			end
		end

		def lock_vault
			@krypt.stop_encryption if @krypt.init
		end

		def cache_files
			@files = {}
			counter = 0
			errors = 0

			Dir.glob("#{@path}/data/*.ree") do |file|
				error = false

				f = File.open(file, 'r')
				encrypted = f.read

				decrypted = @krypt.decrypt_string(encrypted)
				data = YAML.load(decrypted)

				category = data['header']['category']
				@files[data['header']['name']] = DataFile.new(data['header']['name'], self,"#{category}", data)
				# TODO: If there is an error set the error variable to true

				errors += 1 if error
				counter += 1
			end

			ReeLogger.write("Vault #{@name} cached #{counter} files with #{errors} errors!", 'info')
		end

		def cache_file_with_name(name)
		end

		def change_password(current_pw, new_pw)
			stored = @config['phash']
			if stored == Robocrypt::CryptUtils.hash_base512("#{current_pw}")
				ReeLogger.write("User authentication was successful. Now attempting to cipher shift on vault #{@name}", 'warn')
				@krypt.init_shift(new_pw)
				VaultLogger.write("A cipher shift round has started. On error the vault WILL be corrupted!", 'warn')

				update_dataset # => Cache everything!

				@files.each do |name, file|
					touch_name = file.name
					touch_path = file.path

					f = File.open("#{touch_path}", 'r')
					untouched = f.read

					VaultLogger.write("Shifting cipher for #{touch_name}", 'warn')
					shifted = @krypt.shift_cipher(untouched, "#{new_pw}")
					File.open("#{touch_path}", "w"){ |touched| touched.write(shifted) }
					VaultLogger.write("Shift for #{file.name} successful", 'warn')
				end
				new_key = @krypt.finalise_shift
				pw_hash = Robocrypt::CryptUtils.hash_base512("#{new_pw}")
				VaultLogger.write("Cipher shift for vault #{@name} was successful. Writing change to config!", 'info')
				@config['khash'] = "#{new_key}"
				@config['phash'] = "#{pw_hash}"

				File.open("#{@path}/global.yml", "w"){ |f| YAML.dump(@config, f) }
				ReeLogger.write("Updating Vault configuration.", "debug")

				ReeLogger.write("Cipher shift for vault #{@name} completed successfully. Re-caching all files!", 'info')
				unlock_vault("#{new_pw}") # => Cache config takes the unhashed password because it authenticates the krypto module!
				update_dataset # => Update files to
			end
		end

		def update_dataset
			cache_header
			cache_files
		end

		def add_file(name, category)
			if @files["#{name}"] == nil
				@files["#{name}"] = DataFile.new("#{name}", self, "#{category}")
			else
				ReeLogger.write("File '#{name}' already exists in vault '#{@name}'", "debug")
			end
			update_config
			update_dataset
		end

		def remove_file(name)
			if @files["#{name}"] != nil
				FileUtils.rm(@files["#{name}"].path)
				@files["#{name}"] = nil
				update_dataset
			end
		end

		def read_file(name)
			(return @files["#{name}"]) if @files["#{name}"] != nil
		end

		def update_file(name, field_name, field_content, parent = "contents")
			@files["#{name}"].write("#{field_name}", "#{field_content}", "#{parent}").close
			update_config
			update_dataset
		end

		def transpose
			trans = Transposer.new(self)
			trans.flush(Environment::OUT)
		end

		# => Updates the configuration edit date and device.
		def update_config
			@config = YAML.load_file("#{@path}/global.yml")
			@config['last_updated'] = "#{ReeTime.get_time}"
			@config['updating_machine'] = "#{Socket.gethostname}"

			# => Write to file
			File.open("#{@path}/global.yml", "w"){ |f| YAML.dump(@config, f) }
			ReeLogger.write("Updating Vault configuration.", "debug")
			update_dataset
		end
	end
end