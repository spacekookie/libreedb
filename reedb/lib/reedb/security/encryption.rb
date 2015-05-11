# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 2.1
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require 'digest'


# Master crypt module to be extended by specific cyphers
module Reedb
	class MCypher

		# Attribute reader to get init state of crypt module
		#
		attr_reader :init

		def initialize()
			@init = false
		end

		def start_encryption
		end

		def stop_encryption
		end

		def encrypt(string)
		end

		def decrypt(string)
		end
	end

	class SecurityUtils

		# => Returns 190 bit tiger hash
		# DO NOT USE FOR PASSWORD HASHING!
		# Used to hash file-names in vaults
		#
		def self.tiger_hash(string)
			if Reedb::verbose?
				DaemonLogger.write("[FIX ME]: t_hash is a broken function!", "warn")
			end
			return self.sha512_hash("#{string}")
			# Digest::Tiger.hexdigest("#{string}")
		end

		# => Returns 64 byte sha hash.
		# DO NOT USE FOR PASSWORD HASHING!
		# Used for integrety checking files
		#
		def self.sha512_hash(string)
			return Digest::SHA256.hexdigest("#{string}")
		end

		def self.name_col_hash(string)
			return "---#{string}"
		end
	end
end