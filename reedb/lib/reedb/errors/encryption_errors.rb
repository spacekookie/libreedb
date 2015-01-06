# ====================================================
# Copyright 2014 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the MIT License.
# (See accompanying file LICENSE or copy at
# http://opensource.org/licenses/MIT)
# ====================================================


class MissingEncryptionTypeError < StandardError
end

class MissingUserPasswordError < StandardError
end

class InvalidUserPasswordError < StandardError
end

class InvalidDecryptionKeyError < StandardError
end


### PLAYGROUND ###


# def do_something_dangerous(var)
# 	if var == 0
# 		raise MissingEncryptionTypeError.new, "Invalid identifier"
# 	end
# end

# begin
# 	do_something_dangerous(0)
# rescue MissingEncryptionTypeError => e
#  	puts e.message
# end