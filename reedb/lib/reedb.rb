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
		# Is only split into two values: unix and win. Determines a lot of file
		# processes and manipulations in a vault.
		#
		def archos() (return @@testvar) end

		def passlength() (return @@passlength) end

		def init(global_os, passlength = 8)
			@@testvar = global_os
			@@passlength = passlength

			if global_os == :win
				# => Do windows shit here
			else
				# => Do unix snazzle here
			end
		end

		# Default encryption type is 'AES'
		#
		def vault(name='default', path=nil, encrypt='auto_fill')
			return ReeVault.new(name, path, encrypt)
		end
	end
end

user_pw = "1234567890123"

Reedb.init(:unix, 12) # => defines OS and minimal password length on vault
path = File.expand_path('~/Desktop/')

# Default encryption is set to 'aes'
Reedb.vault(name='default', "#{path}", :aes).secure_config(true).create(user_pw)