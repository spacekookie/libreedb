# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Internal requirements
require_relative 'reedb/errors/daemon_errors'

require_relative 'reedb/utils/meta_vault'
require_relative 'reedb/utils/utilities'
require_relative 'reedb/utils/logger'
require_relative 'reedb/utils/uuids'

require_relative 'reedb/constants'
require_relative 'reedb/reevault'


# System requirements
require 'securerandom'

module Reedb
	class << self
		# Returns the platform/ architecture of the daemon and vault handler

		def archos() (return @@archos) end

		# Returns whether or not this is a daemon
		def daemon?() (return @@daemon) end

		# Returns the minimum passphrase length is
		def passlength() (return @@pw_length) end

		# Returns whether verbose mode is enabled
		def verbose?() (return @@verbose) end			
	end


	def self.included(api)
		class << api

			@@counter = 0
			@@started = false

			# Some more runtime variables
			@@path = Reedb::DEF_MASTER_PATH
			@@config_path = Reedb::DEF_MASTER_PATH
			@@no_token = false
			@@verbose = false
			@@pw_length = -1
			@@cleanup = true
			@@daemon = true
			@@archos = nil
			
			# Stores the active vaults
			@@active_vaults = {}

			# Stores the active tokens
			@@tokens = {}

			# Stores the master configuration
			@@config = nil

			# PRIVATE FUNCTIONS BELOW
			private

			# Generates an authentication token for vault access.
			# The function also adds the token, bound to the vault name, to the
			# @@tokens set.
			#
			# Params: 	'name' of the vault
			# 					'path' of the vault
			#
			# => Base64 encoded token
			#
			def generate_token(uuid, path)
				# Concatinates the token together and base64 encodes it
				token = Base64.encode64("#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}--#{uuid}--#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}--#{path}--#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}")
				@@tokens[token] = [] unless @@tokens.include?(token)
				@@tokens[token] << uuid
				update_tracked_vault("#{uuid}", nil, nil, nil, token)
				return token
			end

			# Writes a vault into a tracking file for the user
			# That means that the vault will be tracked next time
			# Reedb starts
			#
			# Params: 	name => Public vault name
			# 					path => Location on the system
			# 					tokens => Authentication token for applications
			#
			def track_vault(name, path, size, uuid)
				@@config[:vaults]["#{uuid}"] = {} unless @@config[:vaults].include?(uuid)

				# Adds actual size as soon as the vault gets unlocked by an application
				@@config[:vaults]["#{uuid}"][:meta] = MetaVault.new("#{name}", "#{path}", size, "#{uuid}")
				@@config[:vaults]["#{uuid}"][:tokens] = [] unless @@config[:vaults]["#{uuid}"][:tokens]
				@@config[:vaults]["#{uuid}"][:tokens] = []

				write_config
			end

			def untrack_vault uuid
				@@config[:vaults]["#{uuid}"] = nil if @@config[:vaults].include?(uuid)
			end

			def update_tracked_vault(uuid, name, path, size, token)
				return nil unless @@config[:vaults].include?(uuid)

				@@config[:vaults]["#{uuid}"][:meta].name = name if name
				@@config[:vaults]["#{uuid}"][:meta].path = path if path
				@@config[:vaults]["#{uuid}"][:meta].size = size if size

				@@config[:vaults]["#{uuid}"][:tokens] = [] unless @@config[:vaults]["#{uuid}"][:tokens]
				@@config[:vaults]["#{uuid}"][:tokens] << token if token

				write_config
			end

			# Removes a token from a vault config thus removing any access that token had.
			#
			def remove_token(uuid, token)
				return nil unless @@config[:vaults].include?(uuid)
				return nil unless @@config[:vaults]["#{uuid}"][:tokens].include?(token)
				
				@@config[:vaults]["#{uuid}"][:tokens].delete(token)
				write_config
			end

			# Caches the config file to @@config
			#
			#
			def cache_config
				# Now read vault information and put it into @@active_vaults field
				if File.exist?("#{@@config_path}")
					read_config
				else
					# Creates some dummy info
					@@config = {}
					@@config[:global] = {}
					@@config[:global][:logs] = :default
					@@config[:vaults] = {}

					# Writes the config to file with Base64 encoding
					write_config
					FileUtils::chmod_R(0744, "#{@@config_path}")
				end

				check_vault_integreties if @@cleanup

				# At this point @@config has been loaded
				vault_count = @@config[:vaults].size
				DaemonLogger.write("Found #{vault_count} vault(s) on the system.", "debug")
			end

			# Check vault integreties here!
			# Will try every vault in the config and remove the ones that are no longer 
			# available to avoid errors and access corruptions
			#
			def check_vault_integreties
				# Vaults that will be marked for removal
				marked = []
				@@config[:vaults].each do |uuid, data|
					unless ReeVault.new(data[:meta].name, data[:meta].path, :auto).try?
						marked << uuid
					end
				end

				marked.each do |uuid|
					# puts "Removing: #{uuid}"
					DaemonLogger.write("Removing corrupted vault #{uuid}", 'warn')
					@@config[:vaults].delete(uuid)
				end
				write_config
			end

			def write_config
				data = Marshal.dump(@@config)
				File.open(@@config_path, 'wb+') { |file| file.write(data) }
				read_config
			end

			def read_config
				data = File.open(@@config_path, "rb").read()
				@@config = Marshal.load(data)
			end
		end
	end

	# Core submodule of the Interface stack
	module Core
		include Reedb

		class << self

			def init(options)
				@@daemon = if options.include?(:daemon) then options[:daemon] else true end
				@@archos = options[:os]
				@@pw_length = options[:pw_length]
				@@verbose = if options.include?(:verbose) then options[:verbose] else false end
				@@no_token = if options.include?(:no_token) then options[:no_token] else false end

				if @@no_token
					puts "NO_TOKEN mode has not been implemented yet! Please just use token authentication mode."
					@@no_token = false
				end
				
				@@path = if options.include?(:path) then options[:path] else "&&HOME&&" end

				# Enable cleanup mode
				# This means that the config will be cleaned and unused tokens removed
				# instead of leaving them in the file and configurations.
				# It is recommended to use cleanup mode.
				#
				@@cleanup = if options.include?(:cleanup) then options[:cleanup] else true end

				# Set of vaults that map a VaultBuffer to the vault itself.
				# Never exposed outside the API
				#
				@@active_vaults = {}
				
				# List of tokens authorised for this daemon. Maps tokens to vault names.
				# This is used for authentication and never exposed to the outside API
				# NOTE! This is not used when the :no_token option is enabled
				#
				@@tokens = {} unless @@no_token

				if @@archos == :linux
					master_path = File.expand_path('~/.config/reedb/')

					# Puts the folder in /etc if running as root
					master_path = "/etc/reedb" if Utilities::parse_user == 'root'

					log_path = File.expand_path('~/.config/reedb/logs/')
					@@config_path = File.join("#{master_path}", "master.cfg")

				elsif @@archos == :osx
					master_path = File.expand_path('~/Library/Application\ Support/reedb/')
					log_path = File.expand_path('~/Library/Application\ Support/reedb/logs/')
					@@config_path = File.join("#{master_path}", "master.cfg")
				else
					# Windows crap
				end

				# Changing file permissions. Does this do ANYTHING on windows?
				FileUtils::mkdir_p("#{log_path}") # 744 (owned by $USER)
				FileUtils::chmod_R(0744, "#{log_path}")
				FileUtils::chmod_R(0744, "#{master_path}")
				
				Reedb::DaemonLogger.setup("#{log_path}")
				@@started = true
				Reedb::DaemonLogger.write("Reedb was started successfully. Reading vault information now...", 'debug')

				# Now cache the config
				cache_config

				# Open debounce tread and mirror current vault information onto it.
				# debounce_handler
			end


			def terminate(reason = nil)
				puts "Must start process first" unless @@started

				new_reason = "unknown reason"
				new_reason = "a user request" if reason == "user"
				new_reason = "a system request" if reason == "root"

				# My first easter-egg. Yay! :)
				new_reason = "the illuminati" if reason == "aliens"

				DaemonLogger.write("[TERMINATION]: Scheduling termination because of #{new_reason}.")

				# TODO: Close the debounce thread here.

				@@started = false
				# Closing open vaults here
				counter = 0 ; @@active_vaults.each { |k, v| v.close; counter += 1 }
				DaemonLogger.write("[TERMINATION]: Closed #{counter} vaults. Done!")
			end
		end
	end

	# Config submodule of the Interface stack
	module Config
		module Master
			include Reedb
			class << self

				# Set a global timeout time to all vaults. Determine when a vault will
				# be unloaded or the daemon will lock itself.
				# Time provided in seconds
				#
				def global_timeout dt
				end

				# Set the log state of the daemon. That determines what error calls will
				# be logged and what will be ommited.
				#
				def logging_state state
				end

				# Define a minimal passphrase length for vaults to have. This can
				# break access to vaults if they were created on a different system
				# so be careful with this!
				# 
				def passphrase_length length
				end

				# Cleans the config file of broken vaults and config items. This is
				# being done by default when operating in cleanup mode.
				#
				def clean_config

				end
			end
		end

		module Vault
			include Reedb
			class << self
				def set_vault_timeout dt
				end

				def add_header_field(field, type)
				end

				def change_passphrase(old_phrase, new_phrase)
				end

				def read_config
				end
			end
		end
	end

	# Vault submodule of the Interface stack
	module Vault
		include Reedb

		class << self

			# Returns a list of all vaults tracked by Reedb (by the current user).
			# Vaults are ordered in a dictionary under their UUID that's used by the
			# Reedb daemon.
			#
			# => A compiled JSON of vaults with uuid, name, path and size
			#
			def available_vaults
				available = {}

				@@config[:vaults].each do |uuid, value|
					# puts @@config[:vaults]["#{uuid}"]
					available["#{uuid}"] = {}
					available["#{uuid}"][:name] = value[:meta].name
					available["#{uuid}"][:path] = value[:meta].path
					available["#{uuid}"][:size] = value[:meta].size
				end
				return available
			end

			# Creates a new vault on the current system. Returns nil if vault already existed at
			# location. Returns a token if the creation and authentication was successful on the
			# user side.
			# Also adds that token to the @@tokens list
			#
			# THROWS A LOT OF EXCEPTIONS!
			#
			#
			# Params: 	
			# 				'name' of the vault
			# 				'path' of the vault
			# 				user 'passphrase'
			# 				'encryption' method (:aes, :twofish, :auto)
			#
			# => Base64 encoded token | nil if errors occured.
			#
			def create_vault(name, path, passphrase, encryption = :auto)
				# Creates new UUIDs until one is found that doesn't already exist in the scope
				uuid = nil
				loop do
					uuid = UUID::create_v1
					(break) unless @@config[:vaults].include?(uuid)
				end

				# Create actual Vault object
				# This throws errors!
				tmp_vault = ReeVault.new("#{name}", "#{path}", encryption).create("#{passphrase}")

				# Creates a metavault with name, path, size and uuid to be tracked on the system
				# metavault = MetaVault.new("#{name}", "#{path}", 0, "#{uuid}")

				# Adds vault to the active set of vaults
				@@active_vaults[uuid] = tmp_vault

				# Generates a token
				unless @@no_token
					token = generate_token(uuid, path)
					track_vault(name, path, 0, uuid)
					return token
				end

				# In case token authentication was disabled
				track_vault(uuid, nil)
				return nil
			end


			# ONLY PERMITTED WHEN IN NO_TOKEN MODE! WILL BE DISABLED AUTOMATICALLY WHEN
			# USING THE HTTP MODULE!
			#
			# Loads a vault with a UUID and passphrase into the current vault set.
			# Ignores the token set (as not applicable when in token mode) and simply
			# returns a confirmation that vault access was granted.
			#
			def access_vault(uuid, passphrase)
				raise FunctionNotImplementedError.new, "This has not been implemented yet! Use token authentication via the DAEMON module."
				return false
			end


			# Removes a vault from the file system. This requires special privileges 
			# to do via this interface to prevent deleting vaults without the users 
			# permission. Will also fire a user interrupt to alert them of this 
			# behaviour depending on platform.
			#
			# Params:   
			#       		'uuid' of the vault
			#       		'passphrase' of the vault
			#       		'token' of an application that needs to be authorised
			#
			# => Returns boolean describing success
			#
			def remove_vault(uuid, passphrase, token)
				# Returns false if that token isn't authorised
				return false unless @@tokens[token].include?(uuid)

				# Return false if vault has never been scoped before
				return false unless @@config[:vaults].include?(uuid)

				# Return false if vault is currently locked
				return false if @@active_vaults[uuid].locked?
	 
				# Mark a vault for removal and only actually
				raise FunctionNotImplementedError.new, "This has not been implemented yet! Don't do this."
				return false
			end

			# Adds a new vault to the tracking scope of this Reedb daemon. Does not grant access
			# or generate a new token for application interaction.
			# On a new install usually called just before requesting a token
			# 
			# Params: 		'name' of the vault
			# 				'path' on the systen
			#
			def scope_vault(name, path)
				# Checks if that specific vault was already scoped
				@@config[:vaults].each do |key, value|
					if value[:meta].name == "#{name}" && value[:meta].path == "#{path}"
						DaemonLogger.write("Vault already scoped at #{path}", 'info')
						raise VaultAlreadyScopedError.new, "Vault #{name} already scoped!"
						return false
					end
				end
				vault = ReeVault.new("#{name}", "#{path}", :auto)
				# If it hasn't, proceed here
				if vault.try?
					uuid = nil
					loop do
						uuid = UUID::create_v1
						(break) unless @@config[:vaults].include?(uuid)
					end

					# At this point a vault has been confirmed and a UUID generated
					track_vault(name, path, vault.count, "#{uuid}")
					DaemonLogger.write("Vault successfully scoped at #{path}", 'info')
					cache_config
					return true
				else
					DaemonLogger.write("Tried to scope empty target at #{path}", 'warn')
					raise VaultDoesNotExistError.new, "Tried to scope empty target at #{path}"
					return false
				end
			end

			# Removes a vault from the application scope with a uuid.
			# Closes the vault from the active vault set.
			#
			# Returns nil if no such vault was scoped before.
			#
			def unscope_vault(uuid)
				unless @@config[:vaults]["#{uuid}"]
					raise VaultNotScopedError.new, "Vault #{name} not scoped!"
					return nilp
				end

				path = @@config[:vaults]["#{uuid}"][:path]
				DaemonLogger.write("Unscoping vault #{uuid} at #{path}")
				@@active_vaults["#{uuid}"].close if @@active_vaults["#{uuid}"]
				@@config[:vaults].delete("#{uuid}")
				cache_config
			end

			# Request token for a vault permanently.
			# Only used if @@no_token == false. Unlocks a vault as well
			# with the user passphrase
			#
			# Params: 		'uuid' of the vault
			#       		'search' search queury as described in the wiki
			#
			# => Returns list of headers present in the vault
			#   according to the search queury
			#
			def access_headers(uuid, token, search = nil)
				raise VaultNotAvailableError.new, "The vault you have requested data from is not currently active on this system." unless @@active_vaults["#{uuid}"]

				raise UnknownTokenError.new, "The token you provided is unknown to this system. Access denied!" unless @@tokens[token]

				raise UnautherisedTokenError.new, "The token you provided currently has no access to the desired vault. Access denied!" unless @@tokens[token].include?(uuid)


				return @@active_vaults["#{uuid}"].list_headers(search)
			end


			# Request token for a vault permanently.
			# Only used if @@no_token == false. Unlocks a vault as well
			# with the user passphrase
			#
			# Params:   'uuid' of the vault
			#       		'file_name' file identifier to access
			#
			# => Returns contents (including headers) of a file either
			#   as it's current version or it's edit history.
			#
			def access_file(uuid, token, file_name, history = false)
				raise VaultNotAvailableError.new, "The vault you have requested data from is not currently active on this system." unless @@active_vaults["#{uuid}"]

				raise UnknownTokenError.new, "The token you provided is unknown to this system. Access denied!" unless @@tokens[token]

				raise UnautherisedTokenError.new, "The token you provided currently has no access to the desired vault. Access denied!" unless @@tokens[token].include?(uuid)
				return @@active_vaults["#{uuid}"].read_file(file_name, history)
			end

			# Request token for a vault permanently.
			# Only used if @@no_token == false. Unlocks a vault as well
			# with the user passphrase
			#
			# Params:   'uuid' of the vault
			#       		'file_name' file identifier to access
			#
			# => Returns contents of a file either as it's current
			#   version or it's edit history
			#
			def insert(uuid, token, file_name, data)
				raise VaultNotAvailableError.new, "The vault you wish to insert data to is not currently active on this system." unless @@active_vaults["#{uuid}"]

				raise UnknownTokenError.new, "The token you provided is unknown to this system. Access denied!" unless @@tokens[token]

				raise UnautherisedTokenError.new, "The token you provided currently has no access to the desired vault. Access denied!" unless @@tokens[token].include?(uuid)

				DaemonLogger.write("Writing data to #{uuid}", 'debug')

				@@active_vaults["#{uuid}"].update(file_name, data)
			end

			# Ends the exchange with a vault. Removes token from active vault record
			#
			def close_vault(uuid, token)
				raise VaultNotAvailableError.new, "The vault you have requested data from is not currently active on this system." unless @@active_vaults["#{uuid}"]

				raise UnknownTokenError.new, "The token you provided is unknown to this system. Access denied!" unless @@tokens[token]

				raise UnautherisedTokenError.new, "The token you provided currently has no access to the desired vault. Access denied!" unless @@tokens[token].include?(uuid)

				DaemonLogger.write("Closing vault with #{uuid}.", "debug")

				# Close the vault
				@@active_vaults["#{uuid}"].close

				# Delete the vault from the active_vault record with UUID
				@@active_vaults.delete("#{uuid}")

				# TODO: Alert other applications
				# TODO: Don't delete the token if it is being used to access
				# 			other vaults on the system!
				@@tokens.delete(token)

				# Removes token from config
				# TODO: FIX ME?!
				@@config[:vaults]["#{uuid}"][:tokens].delete(token)
				write_config
			end
		end
	end

	module Daemon
		include Reedb

		class << self
			# Request token for a vault permanently.
			# Only used if @@no_token == false. Unlocks a vault as well
			# with the user passphrase
			#
			# Params: 		'name' of the vault
			# 				'passphrase' to unlock
			#
			# => Returns token for vault
			#
			def request_token(uuid, passphrase, parmanent = false)
				# If the vault is not currently open
				unless @@active_vaults.include?(uuid)
					unless @@config[:vaults][uuid]
						DaemonLogger.write("The requested vault is unknown to this system. Aborting operation!", 'error')
						raise VaultNotScopedError.new, "Requested vault #{uuid} is unknown to reedb. Has it been scoped before?"
					end
					# Continue
					name = @@config[:vaults][uuid][:meta].name
					path = @@config[:vaults][uuid][:meta].path
					@@active_vaults[uuid] = ReeVault.new("#{name}", "#{path}", :auto).load(passphrase)
				end
				token = generate_token(uuid, path)
				return token
			end
			# This function should be used by recurring applications that already own
			# an authentication token for a vault.
			#
			def access_with_token(uuid, token, passphrase)
			end
			# Call this function to free a token and remove it from the access
			# tree. This means that the vault it used to access are not removed or
			# unloaded for other applications to use. But the token can no longer
			# be used for file access.
			#
			def free_token(token)
			end
		end #self class end
	end # module Daemon end
end # Module Reedb end

#user_pw = "1234567890123"
#name = "default2"
#path = "/home/spacekookie/Desktop"



# name = "mega2"
# path = "/home/spacekookie"
# passphrase = "omg_awesome_sauce"

# Reedb::Core::init({:os=>:linux, :pw_length=>12})
# token = Reedb::Vault::create_vault(name, path, passphrase, :auto)
# puts token
# puts Reedb::generate_token("something", "blob")

# Reedb::scope_vault(name, path)

# puts Reedb::Vault::available_vaults
# Reedb::Vault::create_vault(name, path, user_pw)

# available = Reedb::Vault::available_vaults
# puts "Available vaults: #{available}\n"

# target = nil ; available.each { |uuid, meta| (target = uuid) if meta[:name] == "default2" }

# puts "Target: #{target}"

# my_token = Reedb::Daemon::request_token(target, user_pw)

# search_qeuery = "tags=awsome#urls=www.lonelyrobot.io"

# headers = Reedb::access_headers(target, my_token, search_qeuery)
# print "#{headers}\n"

# Reedb::Vault::insert(target, my_token, "Lonely Robot", data1)
# Reedb::Vault::insert(target, my_token, "Lonely Robot", data2)
# Reedb::Vault::insert(target, my_token, "Lonely Robot", data3)

# puts Reedb::Vault::access_file(target, my_token, "Lonely Robot", true)

#headers = Reedb::Vault::access_headers(target, my_token)
#puts "Vault headers: #{headers}\n\n"

# Reedb::Vault::close_vault(target, my_token)
# Reedb::Core::terminate("aliens")
