# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.lonelyrobot.io
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

'' '
This is the core file for the Reedb module. It consists of
multiple sub-modules that handle different operations of the
Reedb daemon/ toolkit (when developing with it as a Gem depenency.
' ''

# Internal requirements
require_relative 'reedb/errors/daemon_errors'
require_relative 'reedb/errors/reedb_errors'
require_relative 'reedb/errors/exit_errors'

# Meta information requirements
require_relative 'reedb/utils/meta_vault'
require_relative 'reedb/utils/utilities'
require_relative 'reedb/utils/logger'
# require_relative 'reedb/utils/uuids'

# Internals
require_relative 'reedb/constants'
require_relative 'reedb/debouncer'
require_relative 'reedb/reevault'


# System requirements
require 'securerandom'
require 'uuid'

module Reedb
	def self.included(api)
		class << api

			@@counter = 0
			@@started = false

			# Some more runtime variables
			@@path = Reedb::DEFAULT_PATH
			@@config_path = Reedb::DEFAULT_PATH
			@@no_token = false
			@@verbose = false
			@@pw_length = -1
			@@cleanup = true
			@@daemon = true
			@@archos = nil
			@@lock_file = nil

			# Stores the active vaults
			@@active_vaults = {}

			# Stores the active tokens
			@@tokens = {}

			# Stores the master configuration
			@@config = nil

			# PRIVATE FUNCTIONS BELOW
			private

			@@debouncer = nil
			# @return [Object] Debouncer class to handle updates on vaults
			def debouncer;
				@@debouncer
			end

			@@debounce_thread = nil
			# @return [Object] Debouncer THREAD! to handle updates on vaults
			def debounce_thread;
				@@debounce_thread
			end

			# Method that gets called whenever a change to the currently active vault set occurs.
			# It will give change information to the debounce handler that then updates it's secondary vault set
			# for debouncing.
			# This can UNDER NO CIRCUMSTANCES (!) ever be called from an outside source!
			#
			# @param [String] uuid of the vault
			# @param [String] token for the vault
			# @param [Enum] marker to describe what to do
			#
			# @return nil
			#
			def mirror_debounce(uuid, token, marker)
				return @@debouncer.add_vault(uuid, token) if marker == Reedb::DEB_ADD
				return @@debouncer.remove_vault(uuid) if marker == Reedb::DEB_REM
			end

			# Generates an authentication token for vault access.
			# The function also adds the token, bound to the vault name, to the
			# @@tokens set.
			#
			# Params: 	'name' of the vault
			# 					'path' of the vault
			#
			# @return [String] Base64 encoded token
			#
			def generate_token(uuid, path, permanent = false)
				puts '[WARN] Not implemented!' if permanent

				# Concatinates the token together and base64 encodes it
				token = Base64.encode64("#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}--#{uuid}--#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}--#{path}--#{SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)}")
				token.delete!("\n")

				@@tokens[token] = [] unless @@tokens.include?(token)
				@@tokens[token] << uuid

				update_tracked_vault(uuid, nil, nil, nil, token)
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
				@@config[:vaults][uuid] = {} unless @@config[:vaults].include?(uuid)

				# Adds actual size as soon as the vault gets unlocked by an application
				@@config[:vaults][uuid][:meta] = MetaVault.new("#{name}", "#{path}", size, uuid)
				@@config[:vaults][uuid][:tokens] = [] unless @@config[:vaults][uuid][:tokens]
				@@config[:vaults][uuid][:tokens] = [] # Do I need this?

				write_config
			end

			def untrack_vault(uuid)
				@@config[:vaults].delete(uuid) if @@config[:vaults].include?(uuid)
				write_config
			end

			def update_tracked_vault(uuid, name, path, size, token)
				token.delete!("\n")
				(puts 'ERROR! UUID not in config!'; return nil) unless @@config[:vaults].include?(uuid)

				@@config[:vaults][uuid][:meta].name = name if name
				@@config[:vaults][uuid][:meta].path = path if path
				@@config[:vaults][uuid][:meta].size = size if size

				@@config[:vaults][uuid][:tokens] = [] unless @@config[:vaults][uuid][:tokens]
				@@config[:vaults][uuid][:tokens] << token if token

				write_config
			end

			# Removes a token from a vault config thus removing any access that token had.
			#
			def remove_token(uuid, token)
				token.delete!("\n")
				return nil unless @@config[:vaults].include?(uuid)
				return nil unless @@config[:vaults][uuid][:tokens].include?(token)

				@@config[:vaults][uuid][:tokens].delete(token)
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
				DaemonLogger.write("Found #{vault_count} vault(s) on the system.", 'debug')
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

				# Now save the mess you've made.
				write_config
			end

			def write_config
				data = Marshal.dump(@@config)
				File.open(@@config_path, 'wb+') { |file| file.write(data) }
				read_config
			end

			def read_config
				data = File.open(@@config_path, 'rb').read
				@@config = Marshal.load(data)
			end
		end # Implicit required file, not inspected
	end

	class << self
		# Returns

		@@archos = nil
		# @return [String] the platform/ architecture of the daemon and vault handler
		def archos;
			@@archos
		end

		@@pw_length = -1
		# @return [Integer] the minimum passphrase length is
		def passlength;
			@@pw_length
		end

		@@verbose = false
		# @return [Bool] whether verbose mode is enabled
		def verbose?;
			@@archos
		end


		@@daemon = true
		# @return [Bool] whether Reedb is running as a daemon
		def daemon?
			@@archos
		end
	end

	# Core submodule of the Interface stack
	module Core
		include Reedb

		class << self

			# Initialise Reedb with a set of parameters passed into this method.
			# Please check the wiki to find out what option parameters are
			# available to use here.
			#
			# @param options [Hash] Parameters to run Reedb by sorted in a
			# 							hash datastructure.
			#
			# @return nil
			#
			def init(options, &user_code)
				unless options.include?(:os) && options.include?(:pw_length)
					puts 'Missing :os (:linux|:osx) and/or :pw_length fields from options!'
					exit Reedb::EXIT_OS_PARSE
				end

				# Checks if user code was provided and exits the entire application if not provided!
				if user_code == nil && !options[:daemon]
					puts 'No user function was provided to run! Reedb must run in daemon mode to do that!'
					exit Reedb::EXIT_MISSING_USER_CODE
				end

				@@daemon = options.include?(:daemon) ? options[:daemon] : true
				@@archos = options[:os]
				@@pw_length = options[:pw_length]
				@@verbose = options.include?(:verbose) ? options[:verbose] : false
				@@no_token = options.include?(:no_token) ? options[:no_token] : false
				override = options.include?(:force) ? options[:force] : false

				if @@no_token
					puts 'NO_TOKEN mode has not been implemented yet! Defaulting to token mode.'
					@@no_token = false
				end

				# Now @@path is either a path OR a placeholder.
				@@path = options.include?(:path) ? options[:path] : Reedb::DEFAULT_PATH

				# Enable cleanup mode
				# This means that the config will be cleaned and unused tokens removed
				# instead of leaving them in the file and configurations.
				# It is recommended to use cleanup mode.
				#
				@@cleanup = options.include?(:cleanup) ? options[:cleanup] : true

				# Set of vaults that map a VaultBuffer to the vault itself.
				# Never exposed outside the API
				#
				@@active_vaults = {}

				# List of tokens authorised for this daemon. Maps tokens to vault names.
				# This is used for authentication and never exposed to the outside API
				# NOTE! This is not used when the :no_token option is enabled
				#
				@@tokens = {} unless @@no_token


				# This is called when the default path applies
				if @@path == Reedb::DEFAULT_PATH

					# For good operating systems.
					if @@archos == :linux
						master_path = File.expand_path('~/.config/reedb/')
						master_path = '/etc/reedb' if Utilities::parse_user == 'root'

						log_path = File.expand_path('~/.config/reedb/logs/')

						# For OSX
					elsif @@archos == :osx
						master_path = File.expand_path('~/Library/Application\ Support/reedb/')
						master_path = '/Library/Application\ Support/reedb' if Utilities::parse_user == 'root'

						log_path = File.expand_path('~/Library/Application\ Support/reedb/logs/')

					elsif @@archos == :win
						puts 'This is currently not supported :( Sorry.'
						# Windows crap
					elsif @@archos == :other
						raise UnknownOSError.new, 'Operating system was not specified, yet an override path was not specified either. Can not continue without more information!'
						exit(Reedb::EXIT_OS_PARSE)
					else
						raise UnknownOSError.new, "Could not recognise specified OS. What are you running? Why don't you go here:
 getfedora.org :)"
						exit(Reedb::EXIT_OS_PARSE)
					end
				else
					master_path = @@path
					log_path = File.join("#{master_path}", 'logs')
				end

				# Sets up the config path
				@@config_path = File.join("#{master_path}", 'master.cfg')
				@@lock_file = File.join("#{master_path}", 'lock')

				# Now go create directories if they need to be created.
				FileUtils::mkdir_p("#{log_path}") unless File.directory?("#{log_path}")
				FileUtils::chmod_R(0744, "#{log_path}")
				FileUtils::chmod_R(0744, "#{master_path}")

				# Now that pathing has been established, check if there is a lock file.
				if File.exist?(@@lock_file) && !override
					puts '[FATAL] Another instance of Reedb is already operating from this directory! Choose another directory or terminate the old instance first!'
					exit(Reedb::EXIT_STILL_LOCKED)
				elsif File.exist?(@@lock_file) && override
					puts "There was another instance of Reedb running but you forced my hand. It's dead now..."
				end

				# Create a lock file
				File.open(@@lock_file, 'w+') { |f| f.write('Hier koennte Ihre Werbung stehen.') }

				# Start up the logging service.
				Reedb::DaemonLogger.setup("#{log_path}")

				# Now go and declare this daemon started and log it.
				@@started = true
				Reedb::DaemonLogger.write('Reedb was started successfully. Reading vault information now...', 'debug')

				# Now cache the config
				cache_config

				'' '
				The bottom part initiates the main run look that bounces between debouncer thread and user code.
				This function only returns after all the user code was handled.
				' ''

				# Open debounce object
				@@debouncer = Reedb::Debouncer.new(self)


				# Now actually run the code on two threads and hope that the scheduler does it's job!
				@@debouncerThread = Thread.new { @@debouncer.main }
				user = Thread.new(&user_code)

				# Wait for user code to terminate
				user.join

				# Then call terminate just in case they haven't yet
				Reedb::Core::terminate('null', true)
			end

			# Terminate Reedb with a reason. After calling this function the
			# core functionality (and depending interfaces on the stack)
			# is no longer available.
			#
			# @param reason [String] Reason why Reedb is being terminated to
			# 							be written into central logs.
			#
			# @return nil
			#
			def terminate(reason = nil, silent = false)
				puts 'Must start process first' if !@@started && !silent
				return unless @@started

				new_reason = 'unknown reason'
				new_reason = 'a user request' if reason == 'user'
				new_reason = 'a system request' if reason == 'root'

				# My first easter-egg. Yay! :)
				new_reason = 'the illuminati' if reason == 'aliens'

				DaemonLogger.write("[TERM]: Scheduling termination because of #{new_reason}.")

				# Let the debouncer thread time out
				@@debouncer.running = false
				sleep(Reedb::THREAD_TIMEOUT_TIME)

				# Then join it and be done with it
				@@debouncerThread.join

				# Closing open vaults here
				counter = 0; @@active_vaults.each do |uuid, vault|
					vault.close
					@@config[:vaults][uuid][:tokens].each { |token| Reedb::Daemon::free_token(token) }
					counter += 1
				end
				write_config

				# Then declare Reedb terminated and remove the lock file
				@@started = false
				File.delete(@@lock_file) if File.exist?(@@lock_file)
				DaemonLogger.write("[TERM]: Closed #{counter} vaults. Done!")
				return 0
			end
		end
	end # module core end.

	# Config submodule of the Interface stack
	module Config
		module Master
			include Reedb
			class << self

				# Set a global timeout time to all vaults. Determine when a vault will
				# be unloaded or the daemon will lock itself.
				# Time provided in seconds
				#
				def global_timeout(dt)
					Reedb::debouncer.set_custom_timeout(dt)
				end

				# Set the log state of the daemon. That determines what error calls will
				# be logged and what will be ommited.
				#
				def logging_state(state)
					state << ''
				end

				# Define a minimal passphrase length for vaults to have. This can
				# break access to vaults if they were created on a different system
				# so be careful with this!
				#
				def passphrase_length(length)
					length << ''
				end

				# Because Reedb can be operated out of different paths at runtime
				# (or initiation) this function in the Config::Master interface
				# returns the path that this Reedb instance is keeping it's config.
				#
				# Because of this, external applications can write their configs into
				# $(OPERAND_PATH)/apps if they do not bring their own configs directory.
				#
				# @return op_path
				#
				def get_operation_path
				end

				# Cleans the config file of broken vaults and config items. This is
				# being done by default when operating in cleanup mode.
				#
				def clean_config

				end
			end # self class end
		end # module Config::Master end

		module Vault
			include Reedb
			class << self
				def set_vault_timeout(dt)
					dt << ''
				end

				def add_header_field(field, type)
					field << ''
					type << field
				end

				def change_passphrase(old_phrase, new_phrase)
					old_phrase << ''
					new_phrase << '\n'
				end

				def read_config
				end
			end # self class end
		end # module Config::Vault end
	end # module config end

	# Vault submodule of the Interface stack
	module Vault
		include Reedb

		class << self

			# Returns a list of all vaults tracked by Reedb (by the current user).
			# Vaults are ordered in a dictionary under their UUID that's used by the
			# Reedb daemon.
			#
			#
			# @return available_vaults [Hash]
			#
			def available_vaults
				available = {}

				@@config[:vaults].each do |uuid, value|
					# puts @@config[:vaults][uuid]
					available[uuid] = {}
					available[uuid][:name] = value[:meta].name
					available[uuid][:path] = value[:meta].path
					available[uuid][:size] = value[:meta].size
				end
				return available
			end

			# Creates a new vault on the current system. Returns nil if vault
			# already existed at location. Returns a token if the creation
			# and authentication was successful on the user side.
			#
			# Also adds that token to the @@tokens list.
			#
			# ! THROWS A LOT OF EXCEPTIONS !
			#
			# @param name [String] Name of the vault
			# @param path [String] Path of the vault
			# @param passphrase [String] User passphrase for decryption
			# @param encryption [enum] Encryption method (:aes, :twofish, :auto)
			#
			# @return token [Base64 String]
			#
			def create_vault(name, path, passphrase, encryption = :auto)
				# Creates new UUIDs until one is found that doesn't already exist in the scope
				uuid = UUID.new
				loop do
					uuid = uuid.generate
					(break) unless @@config[:vaults].include?(uuid)
				end

				# Create actual Vault object
				# This throws errors!
				tmp_vault = ReeVault.new("#{name}", "#{path}", encryption).create("#{passphrase}")

				# Creates a metavault with name, path, size and uuid to be tracked on the system
				# metavault = MetaVault.new("#{name}", "#{path}", 0, uuid)

				# Adds vault to the active set of vaults
				@@active_vaults[uuid] = tmp_vault

				# Track the vault in the config
				track_vault(name, path, 0, uuid)

				# Generate a token
				token = generate_token(uuid, path)

				# Generate a token for the debouncer and remove it if the vault was already accessable
				debouncer_token = generate_token(uuid, path)
				check = mirror_debounce(uuid, debouncer_token, Reedb::DEB_ADD)
				Reedb::remove_token(uuid, debouncer_token) unless check

				# Then return the token for the user
				token.delete!("\n")
				return token
			end

			# !!! NOT IMPLEMENTED YET !!!
			#
			# ONLY PERMITTED WHEN IN NO_TOKEN MODE!
			# Loads a vault with a UUID and passphrase into the current vault set.
			# Ignores the token set (as not applicable when in token mode) and simply
			# returns a confirmation that vault access was granted.
			#
			def access_vault(uuid, passphrase)
				raise MissingTokenError.new, 'Reedb is running in token mode. Please specify a token via the DAEMON module
access handler' unless @@no_token

				raise FunctionNotImplementedError.new, 'This has not been implemented yet! Use token authentication via the DAEMON module.'
				return false

				# debouncer_token = generate_token(uuid, path)
				# check = Reedb::mirror_debounce(uuid, debouncer_token, Reedb::DEB_ADD)
				# Reedb::remove_token(uuid, debouncer_token) unless check

			end

			# Removes a vault from the file system. This requires special privileges
			# to do via this interface to prevent deleting vaults without the users
			# permission. Will also fire a user interrupt to alert them of this
			# behaviour depending on platform.
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param passphrase [String] User passphrase for decryption
			# @param token [String] Authentication token for validation
			#
			# @return success [Boolean]
			#
			def remove_vault(uuid, passphrase, token)
				token.delete!("\n")
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

			# Adds a new vault to the tracking scope of this Reedb daemon.
			# Does not grant access or generate a new token for application interaction.
			# On a new install usually called just before requesting a token
			#
			# @param name [String] Name of the vault
			# @param path [String] Path of the vault
			#
			# @return success [Boolean]
			#
			def scope_vault(name, path)
				# Checks if that specific vault was already scoped
				@@config[:vaults].each do |_, value|
					if value[:meta].name == "#{name}" && value[:meta].path == "#{path}"
						DaemonLogger.write("Vault already scoped at #{path}", 'info')
						raise VaultAlreadyScopedError.new, "Vault #{name} already scoped!"
						return false
					end
				end
				vault = ReeVault.new("#{name}", "#{path}", :auto)
				# If it hasn't, proceed here
				if vault.try?
					uuid = UUID.new
					loop do
						uuid = uuid.generate
						(break) unless @@config[:vaults].include?(uuid)
					end

					# At this point a vault has been confirmed and a UUID generated
					track_vault(name, path, vault.count, uuid)
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
			# @param uuid [String] UUID of a vault (as an ID)
			#
			# @return success [Boolean]
			#
			def unscope_vault(uuid)
				unless @@config[:vaults][uuid]
					raise VaultNotScopedError.new, "Vault #{name} not scoped!"
					return nil
				end

				path = @@config[:vaults][uuid][:path]
				DaemonLogger.write("Unscoping vault #{uuid} at #{path}")
				@@active_vaults[uuid].close if @@active_vaults[uuid]
				@@config[:vaults].delete(uuid)
				cache_config
			end

			# List headers from a vault with a search qeury (nil by default).
			# Can only be used with a token on vaults that authentication has
			# been successful before.
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param token [String] Authentication token for access
			# @param search [String] Search qeury as described in the wiki
			#
			# @return headers [List]
			#
			def access_headers(uuid, token, search = nil)
				token.delete!("\n")

				raise VaultNotAvailableError.new, 'The vault you have requested data from is not currently active on this system.' unless @@active_vaults[uuid]
				raise UnknownTokenError.new, 'The token you provided is unknown to this system. Access denied!' unless @@tokens[token]
				raise UnautherisedTokenError.new, 'The token you provided currently has no access to the desired vault. Access denied!' unless @@tokens[token].include?(uuid)

				Reedb::debouncer.debounce_vault(uuid)

				return @@active_vaults[uuid].list_headers(search)
			end

			# Access file contents with a file ID in a vault that a token has already
			# been authenticated on.
			#
			# Depending on the history flag (false by default) the history or only
			# current dataset will be returned.
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param token [String] Authentication token for access
			# @param file_name [String] File identifier
			# @param history [Boolean] history idenfitier
			#
			# @return file [Hash]
			#
			def access_file(uuid, token, file_name, history = false)
				token.delete!("\n")

				raise VaultNotAvailableError.new, 'The vault you have requested data from is not currently active on this system.' unless @@active_vaults[uuid]
				raise UnknownTokenError.new, 'The token you provided is unknown to this system. Access denied!' unless @@tokens[token]
				raise UnautherisedTokenError.new, 'The token you provided currently has no access to the desired vault. Access denied!' unless @@tokens[token].include?(uuid)

				Reedb::debouncer.debounce_vault(uuid)

				return @@active_vaults[uuid].read_file(file_name, history)
			end


			# Access file contents with a file ID in a vault that a token has already
			# been authenticated on.
			#
			# Inserts data into a vault. Depending on parameters and runtime settings
			# this function has different effects. It can be used to create a new file
			# if it doesn't already exists.
			#
			# Please refer to the wiki for details on how to use this function as it
			# (! MAY !) have unwanted side-effects if it is used wrong!
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param token [String] Authentication token for access
			# @param file_name [String] File identifier
			# @param data [Hash] Data that should be written to the vault.
			#
			# @return file [Hash]
			#
			def insert(uuid, token, file_name, data)
				token.delete!("\n")

				raise VaultNotAvailableError.new, 'The vault you have requested data from is not currently active on this system.' unless @@active_vaults[uuid]
				raise UnknownTokenError.new, 'The token you provided is unknown to this system. Access denied!' unless @@tokens[token]
				raise UnautherisedTokenError.new, 'The token you provided currently has no access to the desired vault. Access denied!' unless @@tokens[token].include?(uuid)

				DaemonLogger.write("Writing data to #{uuid}", 'debug')
				@@debouncer.debounce_vault(uuid)

				@@active_vaults[uuid].update(file_name, data)
				return nil
			end

			# Removes a file from a vault that a token was authorised to
			# access. File is identified via an ID handle. This operation
			# is dangerous as it can NOT be reverted!
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param token [String] Authentication token for access
			# @param file_name [String] File identifier
			#
			# @return nil
			#
			def remove(uuid, token, file_name)
				token.delete!("\n")

				raise VaultNotAvailableError.new, 'The vault you have requested data from is not currently active on this system.' unless @@active_vaults[uuid]
				raise UnknownTokenError.new, 'The token you provided is unknown to this system. Access denied!' unless @@tokens[token]
				raise UnautherisedTokenError.new, 'The token you provided currently has no access to the desired vault. Access denied!' unless @@tokens[token].include?(uuid)

				DaemonLogger.write("Writing data to #{uuid}", 'debug')
				Reedb::debouncer.debounce_vault(uuid)

				@@active_vaults[uuid].remove_file(file_name)
				return nil
			end


			# Closes a vault to end the file transaction between you and the vault.
			# The encryption key will be unloaded and scrubbed from memory which means
			# you will have to unlock a vault again
			# (which usually means more user interaction).
			#
			# @param uuid [String] UUID of a vault (as an ID)
			# @param token [String] Authentication token for access
			#
			# @return nil
			#
			def close_vault(uuid, token)
				token.delete!("\n")

				raise VaultNotAvailableError.new, 'The vault you have requested data from is not currently active on this system.' unless @@active_vaults[uuid]
				raise UnknownTokenError.new, 'The token you provided is unknown to this system. Access denied!' unless @@tokens[token]
				raise UnautherisedTokenError.new, 'The token you provided currently has no access to the desired vault. Access denied!' unless @@tokens[token].include?(uuid)

				DaemonLogger.write("Closing vault with #{uuid}.", "debug")

				# Remove the vault from the debouncer
				Reedb::debouncer.remove_vault(uuid)

				# Close the vault
				@@active_vaults[uuid].close

				# Delete the vault from the active_vault record with UUID
				@@active_vaults.delete(uuid)

				# TODO: Alert other applications
				# TODO: Don't delete the token if it is being used to access
				# 			other vaults on the system!
				@@tokens.delete(token)

				# Removes token from config
				@@config[:vaults][uuid][:tokens].delete(token)
				write_config
			end
		end # self class end
	end # module vault end

	module Daemon
		include Reedb

		class << self

			# Request token for a vault permanently.
			# Only used if @@no_token == false. Unlocks a vault as well
			# with the user passphrase
			#
			# @param uuid [String] Internal UUID of the vault
			# @param passphrase [String] Passphrase of the vault.
			# @param permanent [Boolean] Indicates whether or not the app intends to come back
			#
			# @return token [Base64 String]
			#
			def request_token(uuid, passphrase, permanent = false)
				# If the vault is not currently open
				unless @@active_vaults.include?(uuid)
					unless @@config[:vaults][uuid]
						DaemonLogger.write('The requested vault is unknown to this system. Aborting operation!', 'error')
						raise VaultNotScopedError.new, "Requested vault #{uuid} is unknown to reedb. Has it been scoped before?"
					end

					# Continue
					name = @@config[:vaults][uuid][:meta].name
					path = @@config[:vaults][uuid][:meta].path
					@@active_vaults[uuid] = ReeVault.new("#{name}", "#{path}", :auto).load("#{passphrase}")
				end
				token = generate_token(uuid, path, permanent)
				puts "Tokens: #{@@tokens}"
				return token
			end

			# This function should be used by recurring applications that already own
			# an authentication token for a vault.
			#
			def access_with_token(uuid, token, passphrase)
				token.delete!('\n')
				raise UnknownTokenError.new, 'Unknown Token!' unless @@config['tokens'][uuid]
			end

			# Call this function to free a token and remove it from the access
			# tree. This means that the vault it used to access are not removed or
			# unloaded for other applications to use. But the token can no longer
			# be used for file access.
			#
			# @param token [String] Token to be freed
			#
			def free_token(token, batch = false)
				token.delete!("\n")

				# Throw a warning if the token isn't valid in the first place.
				raise UnknownTokenError.new, 'The token you provided is unknown to this system' unless @@tokens.include?(token)

				@@tokens[token].each do |uuid|
					@@config[:vaults][uuid][:tokens].delete(token)
				end

				write_config unless batch
			end
		end # self class end
	end # module Daemon end
end # Module Reedb end

# These options should be set when using Reedb as a normal dependency
options = {
	 :daemon => false, # !!! IMPORTANT !!!
	 :os => :linux, # Pick whichever one applies (:linux, :osx, :win, :other)
	 :pw_length => 16, # Is mandatory anyways
	 :force => true, # Overwrites old instances of Reedb that might still be running
	 # :no_token => true, # Doesn't actually do anything yet right now.
	 # :path => "/some/path/here" # !!! IMPORTANT !!!
}

def my_code
	userpw = 'peterpanistderheld'

	all = Reedb::Vault::available_vaults
	tuuid = nil
	all.each { |uuid, data| tuuid = uuid if data[:name] == 'default' && data[:path] == '/home/spacekookie/Desktop' }

	token = Reedb::Daemon::request_token(tuuid, userpw)
	puts "My token: #{token}"

	data = {
		 'body' => {
				'password' => 'mega_secure_password',
				'username' => 'spacekookie'
		 },
		 'header' => {
				'urls' => 'www.lonelyrobot.io',
				'tags' => %w(website games awesome)
		 }
	}

	Reedb::Vault::insert(tuuid, token, 'Lonely Robot', data)
	# Reedb::Vault::create_vault('default', '/home/spacekookie/Desktop', userpw)
end

Reedb::Core::init(options) { my_code }
