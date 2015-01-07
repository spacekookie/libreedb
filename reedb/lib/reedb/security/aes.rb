# # ====================================================
# # Copyright 2014 Random Robot Softworks (see @author)
# # @author: Katharina Sabel | www.2rsoftworks.de
# #
# # Distributed under the MIT License.
# # (See accompanying file LICENSE or copy at
# # http://opensource.org/licenses/MIT)
# # ====================================================

require_relative 'master'
require 'aes'

module Reedb
	class RAES < MCypher
		attr_reader :key

		def initialize
			super
		end

		def init_encryption(password, raw_key = nil)
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
		end

		def stop_encryption
		end

		def encrypt(string)
		end

		def decrypt(string)
		end
	end

end

# aes = Reedb::RAES.new
# aes.init_encryption("abcd")
# puts aes.key

# require 'aes'

# puts AES.key(length=64)


# # Generate a random key
# key = AES.key

# # Encrypt a string.  Default output is base_64 encoded, init_vector and cipher_text are joined with "$"
# b64 = AES.encrypt("A super secret message", key)

# # Same as above but minus the base64 encoding, init_vector and cipher_text are shoved into an array
# plain = AES.encrypt("A super secret message", key, {:format => :plain}) #

# # Generate a random initialization vector
# iv = AES.iv(:base_64)

# # Encrypt a string, with a provided key and init_vector.  
# b64_iv = AES.encrypt("A super secret message", key, {:iv => iv})

# AES.decrypt(b64, key)

# AES.decrypt(plain, key, {:format => :plain})

# # By default data is padded to the nearest 16 bytes block.  To turn
# # this off, you may use the :padding => false (or nil) option.
# #
# # In this mode however, the caller is required to pad the data.  In
# # the following example the message is exactly 16 bytes long, so no
# # error aries.
# msg = AES.encrypt("A secret message", key, {:padding => false})

# # Be sure to pass the same padding option when decrypting the
# # message, as it will fail if you try to decrypt unpadded data and
# # didn't specify :padding => false.
# AES.decrypt(msg, key, {:padding => false})