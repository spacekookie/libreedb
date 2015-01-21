# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Leander Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

require_relative 'encryption'
require 'aes'

module Reedb
	class RAES < MCypher
		def initialize
			super # => Super constructor
		end

		# Starts the encryption and loads a key by either generating a new one
		# or loading an encrypted one from file.
		#
		def start_encryption(password, raw_key = nil)
			if raw_key != nil
				# => Decrypting key with user password
				@key = AES.decrypt(raw_key, password)
			else
				# => Generating new key and encrypting it with user pw
				@key = AES.key
				key_encrypted = AES.encrypt(@key, password)
			end
			
			# => At this point @key should be the unencrypted key!
			@init = true
			return key_encrypted
		end

		# Tries to remove the unencryted key from memory as best as possible.
		# Stops the encryption and prevents further decrypts to occur.
		def stop_encryption
			remove_instance_variable(:@key)
			@init = false
		end

		# Encrypt the clear text using the encryption key
		# Returns a base64 encoded string
		# Throws exceptions
		#
		def encrypt(cleartext)
			begin
				AES.encrypt(cleartext, @key) unless @key.nil?
			rescue Exception => e
				puts e.message
			end
		end

		# Decrypt the cypher text using the encryption key
		# Returns the original clear text.
		# Throws exceptions
		# 
		def decrypt(cyphertext)
			begin
				AES.decrypt(cyphertext, @key) unless @key.nil?
			rescue Exception => e
				puts e.message
			end
		end

		# Starts the shift of the main password and creates a new key (cipher) to encrypt with the new pw
		#
		def init_shift(fresh)
			@tmp_key = AES.key
		end

		# Change the encryption cipher for a file in the vault.
		#
		def shift_cipher(file, fresh)
			temp = AES.decrypt(file, @key) unless @key.nil?
			return AES.encrypt(temp, @tmp_key)
		end

		# Returns new encrypted key
		#
		def finalise_shift
			@key = @tmp_key # => Finishing the cipher shift
			key_encrypted = AES.encrypt(@tmp_key, fresh)
			remove_instance_variable(:@tmp_key)	# => Removing insecure imprint

			return key_encrypted # => To be stored in the new config file!
		end
	end

end