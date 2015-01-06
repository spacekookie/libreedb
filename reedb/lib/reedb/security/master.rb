# ====================================================
# Copyright 2014 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the MIT License.
# (See accompanying file LICENSE or copy at
# http://opensource.org/licenses/MIT)
# ====================================================

# Master crypt module to be extended by specific cyphers
module Reedb
	class MCypher

		# Attribute reader to get init state of crypt module
		#
		attr_reader :init

		def initialize()
			@init = false
		end

		def init_encryption
		end

		def stop_encryption
		end

		def encrypt(string)
		end

		def decrypt(string)
		end

	end
end

		# def init_encryption(password, raw_key = nil)
		# 	@init = false

		# 	if raw_key != nil
		# 		# => Decrypting key with user password
		# 		@key = AES.decrypt(raw_key, password)
		# 	else
		# 		# => Generating new key and encrypting it with user pw
		# 		@key = AES.key
		# 		key_encrypted = AES.encrypt(@key, password)
		# 	end

		# 	# => At this point @key should be the unencrypted key!

		# 	@init = true
		# 	# ReeLogger.write("Loaded encryption key to memory.", "info")
		# 	return key_encrypted # => Returning key to be stored in config file
		# end

		# # Removes the encryption key from memory and disables encryption
		# def stop_encryption
		# 	remove_instance_variable(:@key)
		# 	# ReeLogger.write("Removed encryption key from memory.", "info")
		# end

		# def init_shift(fresh)
		# 	@tmp_key = AES.key
		# 	@key_encrypted = AES.encrypt(@tmp_key, fresh)
		# end

		# def shift_cipher(file, fresh)
		# 	temp = AES.decrypt(file, @key) unless @key.nil?
		# 	return AES.encrypt(temp, @tmp_key)
		# end

		# def finalise_shift
		# 	@key = @tmp_key # => Finishing the cipher shift
		# 	key_encrypted = @key_encrypted

		# 	remove_instance_variable(:@tmp_key)	# => Removing insecure imprint
		# 	remove_instance_variable(:@key_encrypted) # => Removing unneded variables

		# 	return key_encrypted # => To be stored in the new config file!
		# end
		
		# # Encrypt the clear text using the encryption key
		# # Returns a base64 encoded string
		# def encrypt_string(cleartext)
		# 		AES.encrypt(cleartext, @key) unless @key.nil?
		# 		# => ReeLogger.write("Decrypting data at #{ReeTime::get_time}", "debug")
		# 		# TODO: Remove this logger call?
		# 		# => Move this logger call to vault internal logs?
		# 		# => Also...do vault logs get encrypted?
		# end

		# # Decrypt the cypher text using the encryption key
		# # Returns the original clear text 
		# def decrypt_string(cyphertext)
		# 	AES.decrypt(cyphertext, @key) unless @key.nil?
		# end