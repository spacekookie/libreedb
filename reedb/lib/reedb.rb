# ====================================================
# Copyright 2014 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the MIT License.
# (See accompanying file LICENSE or copy at
# http://opensource.org/licenses/MIT)
# ====================================================

# Internal requirements
require_relative "reedb/constants"
require_relative "reedb/reevault"


# External requirements
# require 'http'


# Main module embedded as a system daemon.
# Handles new vaults, loading vaults and caching multiple vaults per
# name and/ or id.
#
module Reedb
	class << self
		def init(os)
			if os == :win
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

		# Really think about what you want here! 
		#
		def cache
		end
	end

	class Wrapper

		def initialize(pid, port, mode)
			if mode == :root
			else
			end

		end

	end
end

user_pw = "09um4jx,okJ=(ZMP)i53,jcRM9ijopj(mpi"
Reedb.vault(name='name', path='~/desktop/') #.create(user_pw) # Default encryption is set to 'aes'

=begin
# db = Reedb.vault(path='~/desktop/name').load('secure password')
# db.close()
=end