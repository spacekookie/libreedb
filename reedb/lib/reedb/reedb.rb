# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Internal requirements
require_relative 'utils/meta_vault'
require_relative 'utils/utilities'
require_relative 'utils/logger'
require_relative 'utils/uuids'
require_relative 'constants'
require_relative 'reevault'

# System requirements
require 'securerandom'

# Main Reedb module that handles the operation of vaults.
# This can be called from a gem dependency from another ruby app
# or used with the reedb daemon.
#
module Reedb
	class << self

		@@counter = 0

		attr_accessor :config

		# Returns the platform/ architecture of the daemon and vault handler
		def archos() (return @@archos) end

		# Returns whether or not this is a daemon
		def daemon?() (return @@daemon) end

		# Returns the minimum passphrase length is
		def passlength() (return @@pw_length) end

		# Returns whether verbose mode is enabled
		def verbose?() (return @@verbose) end

		def terminate reason
			new_reason = "unknown reason"
			new_reason = "user request" if reason == "user"
			new_reason = "system request" if reason == "root"

			DaemonLogger.write("[TERMINATION] Scheduling termination because of #{new_reason}...")
			counter = 0
			@@active_vaults.each { |k, v| v.close; counter += 1 }
			DaemonLogger.write("[TERMINATION] Closed #{counter} vaults. Done!")
		end

		def init(options)
			@@daemon = if options.include?(:daemon) then options[:daemon] else true end
			@@archos = options[:os]
			@@pw_length = options[:pw_length]
			@@verbose = if options.include?(:verbose) then options[:verbose] else false end
			@@no_token = if options.include?(:no_token) then options[:no_token] else false end
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

				FileUtils::mkdir_p("#{log_path}") # 744 (owned by $USER)
				FileUtils::chmod_R(0744, "#{log_path}")
				FileUtils::chmod_R(0744, "#{master_path}")

			elsif @@archos == :osx
				master_path = File.expand_path('~/Library/Application\ Support/reedb/')
				log_path = File.expand_path('~/Library/Application\ Support/reedb/logs/')
				@@config_path = File.join("#{master_path}", "master.cfg")
			else
				# Windows crap
			end
			Reedb::DaemonLogger.setup("#{log_path}")
			Reedb::DaemonLogger.write("Reedb was started successfully. Reading vault information now...", 'debug')
			cache_config

			# Open debounce tread and mirror current vault information onto it.
			debounce_handler
		end

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
		# Params: 	
		# 					'name' of the vault
		# 					'path' of the vault
		# 					user 'passphrase'
		# 					'encryption' method (:aes, :twofish, :auto)
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
			begin
				tmp_vault = ReeVault.new("#{name}", "#{path}", encryption).create("#{passphrase}")
			rescue VaultExistsAtLocationError => e
				DaemonLogger.write("Tried to write over vault '#{name}' at '#{path}'. Action aborted.", 'warn')
				return nil
			end

			# Creates a metavault with name, path, size and uuid to be tracked on the system
			# metavault = MetaVault.new("#{name}", "#{path}", 0, "#{uuid}")

			# Adds vault to the active set of vaults
			@@active_vaults[uuid] = tmp_vault

			# Generates a token
			unless @no_token
				token = generate_token(uuid, path)
				track_vault(name, path, 0, uuid)
				return token
			end

			# In case token authentication was disabled
			track_vault(uuid, nil)
			return nil
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
			puts "THIS HAS NOT BEEN IMPLEMENTED YET! DON'T DO THIS!!11ELEVEN!!!!"
			return false
		end

		# Adds a new vault to the tracking scope of this Reedb daemon. Does not grant access
		# or generate a new token for application interaction.
		# On a new install usually called just before requesting a token
		# 
		# Params: 	'name' of the vault
		# 					'path' on the systen
		#
		def scope_vault(name, path)
			# Checks if that specific vault was already scoped
			@@config[:vaults].each do |key, value|
				if value[:meta].name == "#{name}" && value[:meta].path == "#{path}"
					DaemonLogger.write("Vault already scoped at #{path}", 'info')
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
				return false
			end
		end

		# Removes a vault from the application scope with a uuid.
		# Closes the vault from the active vault set.
		#
		# Returns nil if no such vault was scoped before.
		#
		def unscope_vault(uuid)
			return nil unless @@config[:vaults]["#{uuid}"]
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
		# Params: 	'name' of the vault
		# 					'passphrase' to unlock
		#
		# => Returns token for vault
		#
		def request_token(uuid, passphrase)

			# If the vault is not currently open
			unless @@active_vaults.include?(uuid)
				unless @@config[:vaults][uuid]
					DaemonLogger.write("The requested vault is unknown to this system. Aborting operation!", 'error')
					return nil
				end

				# Continue
				name = @@config[:vaults][uuid][:meta].name
				path = @@config[:vaults][uuid][:meta].path

				@@active_vaults[uuid] = ReeVault.new("#{name}", "#{path}", :auto).load(passphrase)
			end

			#
			# FIGURE OUT A GOOD WAY TO ALERT USER HERE!
			#

			token = generate_token(uuid, path)
			return token
		end

		# This function should be used by recurring applications that already own
		# an authentication token for a vault.
		#
		def access_with_token(uuid, token, passphrase)

		end


		# Request token for a vault permanently.
		# Only used if @@no_token == false. Unlocks a vault as well
		# with the user passphrase
		#
		# Params:   'uuid' of the vault
		#       		'search' search queury as described in the wiki
		#
		# => Returns list of headers present in the vault
		#   according to the search queury
		#
		def access_headers(uuid, token, search = nil)
			return nil unless @@active_vaults["#{uuid}"]
			return nil unless @@tokens[token].include?(uuid)
			@@active_vaults["#{uuid}"].list_headers(search)
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
		def access_file(uuid, token, file_name, history = false)
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
			return Reedb::RETURN_UNKNOWN_TOKEN_ERROR unless @@tokens[token]
			return Reedb::RETURN_UNAUTHORISED_TOKEN_ERROR unless @@tokens[token].include?(uuid)
			return Reedb::RETURN_INACTIVE_VAULT_ERROR unless @@active_vaults["#{uuid}"]

			DaemonLogger.write("Writing data to #{uuid}", 'debug')

			@@active_vaults["#{uuid}"].update(file_name, data)
		end

		# Ends the exchange with a vault. Removes token from active vault record
		#
		def close_vault(uuid, token)
			return nil unless @@tokens[token]
			return nil unless @@tokens[token].include?(uuid)

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

		######################################################
		#
		#
		# PRIVATE FUNCTIONS BELOW
		#
		#
		######################################################

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

		def debounce_handler

		end
	end
end

user_pw = "1234567890123"
name = "default"
path = "/home/spacekookie/Desktop"

Reedb::init({:os=>:linux, :pw_length=>12})
# Reedb::scope_vault(name, path)

# puts Reedb::available_vaults
# Reedb::create_vault(name, path, user_pw)

available = Reedb::available_vaults
puts "Available vaults: #{available}\n\n"

target = nil
available.each do |uuid, meta|
	(target = uuid) if meta[:name] == "default"
end 

#puts "Target: #{target}"

my_token = Reedb::request_token(target, user_pw)
# puts my_token
# puts "My token: #{my_token}\n\n"

data = {
	'header' => {
		'urls' => 'www.lonelyrobot.io',
		'tags' => ['website', 'awsome']
	}
}

Reedb::insert(target, my_token, "Lonely Robot", data)

headers = Reedb::access_headers(target, my_token)
puts "Vault headers: #{headers}\n\n"

Reedb::close_vault(target, my_token)
Reedb::terminate("user")
# data = {
# 	'header'=>{
# 		'url'=>'www.facebook.com',
# 		'category'=>'Web Login'
# 		},
# 	'body'=>{
# 		'password'=>'mega_secure_password',
# 		'username'=>'spacekookie'
# 	}
# }

# Reedb::ReeVault.new(name, path, :aes).load(user_pw).insert("Lonely Robot", data)

##
#####
##########

# available.each do |key, value|
# 	Reedb::unscope_vault("#{key}") if value[:name] == "default2"
# 	#  if value[:name] == 'default2'
# end
# puts Reedb::available_vaults

# token = Reedb::create_vault(name, path, user_pw)
# puts Reedb::available_vaults
# token = Reedb::request_token(name, user_pw)

# puts token

=begin

Reedb.init(:unix, 12) # => defines OS and minimal password length on vault
path = File.expand_path('~/Desktop/reedb')

# Default encryption is set to 'aes'
begin
	Reedb.vault('default', "#{path}", :aes).load(user_pw)
rescue VaultDoesNotExistError, VaultExistsAtLocationError => e
	puts e.message
	puts "If you think this is a bug, please report it <3"
	# exit
end

data = {
	'header'=>{
		'url'=>'www.facebook.com',
		'category'=>'Web Login'
		},
	'body'=>{
		'password'=>'mega_secure_password',
		'username'=>'spacekookie'
	}
}

data2 = {
	'body'=>{
		'password'=>'less_secure',
	}
}=end


Reedb.active_vaults['default'].insert('facebook', data)

=begin
begin
	puts Reedb.active_vaults['default'].read_file('Peter pan')
rescue
	puts "This error was handled: could not read!"
end=end


=begin

begin
	Reedb.active_vaults['default'].remove_file('Peter pan')
rescue
	puts "This error was handled: could not remove!"
end
=end



# beg=begin

# in
# 	Reedb.vault(name='default', "#{path}", :aes).secure_config(true).create(user_pw)

# 	sample_data = {}
# 	sample_data['header'] = {}
# 	sample_data['header']['name'] = "Sample"
# 	sample_data['header']['category'] = "Unsorted"
# 	sample_data['body'] = {}
# 	sample_data['body']['username'] = "spacekookie"
# 	sample_data['body']['password'] = 'the_flying_unicorn_shits_rainbows'

# 	Reedb.active_vaults['default'].insert('Sample', sample_data)

# rescue
# 	puts "Error occured opening your vault. Does it exist?"
# end