# ====================================================
# Copyright 2015 Random Robot Softworks (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

class EncryptionError < StandardError
end

class MissingEncryptionTypeError < EncryptionError
end

class MissingUserPasswordError < EncryptionError
end

class InsecureUserPasswordError < EncryptionError
end