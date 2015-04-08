# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# Internal requirements
require_relative "reedb/constants"
require_relative "reedb/reevault"
require_relative "reedb/utils/utilities"

# Main module embedded as a system daemon.
# Handles new vaults, loading vaults and caching multiple vaults per
# name and/ or id.
#
module Reedb
	class << self

		# Returns the platform/ architecture of the daemon and vault handler
		def archos() (return @@testvar) end

		def passlength() (return @@passlength) end

		def init(global_os, passlength = 8)
			@@testvar = global_os
			@@passlength = passlength

			@@vaults = {}

			if global_os == :win
				# => Do windows shit here
			else
				# => Do unix snazzle here
			end
		end

		def active_vaults
			return @@vaults
		end

		# Default encryption type is 'AES'
		#
		def vault(name='default', path=nil, encrypt='auto_fill')
			@@vaults[name] = ReeVault.new(name, path, encrypt)
			return @@vaults[name]
		end
	end
end

user_pw = "1234567890123"

Reedb.init(:unix, 12) # => defines OS and minimal password length on vault
path = File.expand_path('~/Desktop/')

# Default encryption is set to 'aes'
begin
	Reedb.vault('default', "#{path}", :aes).create(user_pw)
rescue VaultDoesNotExistError => e
	puts e.message
	puts "If you think this is a bug, please report it <3"
	exit
end
data = {'body'=>{'password'=>'mega_secure_password', 'username'=>'spacekookie'}}

Reedb.active_vaults['default'].insert('Peter Pan', data)

# puts Reedb.active_vaults['default'].read_file('Sample File')

# begin
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

puts Reedb.active_vaults['default'].read_file('Peter Pan')

Reedb.active_vaults['default'].close