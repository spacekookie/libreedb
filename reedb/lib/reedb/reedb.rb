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
require 'json'

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

		def init(options)
			@@daemon = if options.include?(:daemon) then options[:daemon] else true end
			@@archos = options[:os]
			@@pw_length = options[:pw_length]
			@@verbose = if options.include?(:verbose) then options[:verbose] else false end
			@@no_token = if options.include?(:no_token) then options[:no_token] else false end
			@@path = if options.include?(:path) then options[:path] else "&&HOME&&" end

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
				@config_path = File.join("#{master_path}", "master.cfg")

				FileUtils::mkdir_p("#{log_path}") # 744 (owned by $USER)
				FileUtils::chmod_R(0744, "#{log_path}")

			elsif @@archos == :osx
				master_path = File.expand_path('~/Library/Application\ Support/reedb/')
				log_path = File.expand_path('~/Library/Application\ Support/reedb/logs/')
				@config_path = File.join("#{master_path}", "master.cfg")
			else
				# Windows crap
			end
			Reedb::DaemonLogger.setup("#{log_path}")
			Reedb::DaemonLogger.write("Reedb was started successfully. Reading vault information now...", 'debug')

			cache_config
		end

		# Returns a list of all vaults tracked by Reedb (by the current user).
		# Note that sensitive information is being abstracted away with this function.
		#
		#
		# => A compiled JSON of vaults with name, path and size
		#
		def available_vaults
			available = {}

			@config[:vaults].each do |key, value|
				available[value[:meta].name] = {}
				available[value[:meta].name][:path] = value[:meta].path
				available[value[:meta].name][:size] = value[:meta].size
			end
			return available
		end

		# Creates a new vault on the current system. Returns nil if vault already existed at
		# location. Returns a token if the creation and authentication was successful on the
		# user side.
		# Also adds that token to the @@tokens list
		#
		# Params: 	
		# 			'name' of the vault
		# 			'path' of the vault
		# 			user 'passphrase'
		# 			'encryption' method (:aes, :twofish, :auto)
		#
		# => Base64 encoded token | nil if errors occured.
		#
		def create_vault(name, path, passphrase, encryption = :auto)
			uuid = UUID::create_v1

			daemon
			begin
				tmp_vault = ReeVault.new("#{name}", "#{path}", encryption).create("#{passphrase}")
			rescue VaultExistsAtLocationError => e
				DaemonLogger.write("Tried to write over vault '#{name}' at '#{path}'. Action aborted.", 'warn')
				return nil
			end
			tmp_meta = MetaVault.new("#{name}", "#{path}", 0)
			@@active_vaults[tmp_meta] = tmp_vault # => Add to @@active_vaults set

			unless @no_token
				token = generate_token(name, path)
				track_vault(name, path, token)
				return token
			end

			# In case token authentication was disabled
			track_vault(name, path, nil)
			return nil
		end

		# Adds a new vault to the tracking scope of this Reedb daemon. Does not grant access
		# or generate a new token for application interaction.
		# On a new install usually called just before requesting a token
		# 
		# Params: 	'name' of the vault
		# 			'path' on the systen
		#
		def scope_vault(name, path)
			tmp_vault = ReeVault.new("#{name}", "#{path}", :auto)
			if tmp_vault.try?
				nv = nanovault(name, path)
				if @config[:vaults].include?(nv)
					DaemonLogger.write("Vault already scoped at #{path}", 'info')
					return false
				else
					# Tracks the vault, writes it to config and then loads the config again
					track_vault(name, path, nil)
				end
			else
				DaemonLogger.write("Tried to scope empty target at #{path}", 'warn')
				return false
			end
			
		end

		# Request token for a vault permanently.
		# Only used if @@no_token == false. Unlocks a vault as well
		# with the user passphrase
		#
		# Params: 	'name' of the vault
		# 			'passphrase' to unlock
		#
		# => Returns token for vault
		#
		def request_token(name, path, passphrase)
			return nil unless @@active_vaults.include?(name)

			#
			# FIGURE OUT A GOOD WAY TO ALERT USER HERE!
			#

			nv = nanovault(name, path)
			token = generate_token(name, path)
			return nil if @@tokens.include?(token)

			
		end

		# Used to access a vault with a specific token
		#
		# => A vault if the token was legitimate. Nil if it was not
		#
		def access_vault_with_token(name, path, token)

		end

		# Ends the exchange with a vault. Removes token from active vault record
		#
		def close_vault(vault, token)
			return nil unless @@tokens[token].include?(vault)
			@@vaults[vault].close
			@@vaults[vault] = nil

			# Removes the token from authentication
			@@tokens[token] = nil
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
		# 			'path' of the vault
		#
		# => Base64 encoded token
		#
		def generate_token(name, path)
			rnd = SecureRandom.base64(Reedb::TOKEN_BYTE_SIZE)

			# Concatinates the token together and base64 encodes it
			token = Base64.encode64("#{name}::#{path}::#{rnd}")
			@@tokens[token] = [] unless @@tokens.include?(token)
			@@tokens[token] << "#{name}"
			return token
		end

		# Writes a vault into a tracking file for the user
		# That means that the vault will be tracked next time
		# Reedb starts
		#
		# Params: 	name => Public vault name
		# 			path => Location on the system
		# 			tokens => Authentication token for applications
		#
		def track_vault(name, path, token)
			nv = nanovault(name, path)
			@config[:vaults][nv] = {} unless @config[:vaults].include?(nv)

			# Adds actual size as soon as the vault gets unlocked by an application
			@config[:vaults][nv][:meta] = MetaVault.new(name, path, -1)
			@config[:vaults][nv][:tokens] = [] unless @config[:vaults][nv][:tokens]
			@config[:vaults][nv][:tokens] << token

			write_config
		end

		def untrack_vault(name, path)
			nv = nanovault(name, path)
			@config[:vaults][nv] = nil if @config[:vaults].include?(nv)
		end

		def update_tracked_vault(name, path, size, token)
			nv = nanovault(name, path)
			return nil unless @config[:vaults].include?(nv)

			@config[:vaults][nv][:meta].name = name if name
			@config[:vaults][nv][:meta].path = path if path
			@config[:vaults][nv][:meta].size = size if size

			@config[:vaults][nv][:tokens] = [] unless @config[:vaults][nv][:tokens]
			@config[:vaults][nv][:tokens] << token if token

			write_config
		end

		# Removes a token from a vault config thus removing any access that token had.
		#
		def remove_token(name, path, token)
			nv = nanovault(name, path)

			return nil unless @config[:vaults].include?(nv)
			return nil unless @config[:vaults][nv][:tokens].include?(token)
			
			@config[:vaults][nv][:tokens].delete(token)
			write_config
		end

		def nanovault(name, path)
			return "#{name}::#{path}"
		end

		# Caches the config file to @config
		#
		#
		def cache_config
			# Now read vault information and put it into @@active_vaults field
			if File.exist?("#{@config_path}")
				read_config
			else
				# Creates some dummy info
				@config = {}
				@config[:global] = {}
				@config[:global][:logs] = :default
				@config[:vaults] = {}

				# Writes the config to file with Base64 encoding
				write_config
			end

			# At this point @config has been loaded
			@config[:vaults].each do |key, value|

			end
		end

		def write_config
			data = Marshal.dump(@config)
			File.open(@config_path, 'wb+') { |file| file.write(data) }
			# File.open(@config_path, "wb+").write(str)
			read_config
		end

		def read_config
			data = File.open(@config_path, "rb").read()
			@config = Marshal.load(data)
		end
	end
end

# user_pw = "1234567890123"
# name = "default"
# path = "/home/spacekookie/Desktop"
# 
# Reedb::init({:os=>:linux, :pw_length=>12})
# # Reedb::scope_vault(name, path)
# puts Reedb::available_vaults

# puts Reedb::available_vaults
# token = Reedb::create_vault(name, path, user_pw)
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