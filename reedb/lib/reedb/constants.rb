# ====================================================
# Copyright 2015 Lonely Robot (see @author)
# @author: Katharina Sabel | www.2rsoftworks.de
#
# Distributed under the GNU Lesser GPL Version 3
# (See accompanying LICENSE file or get a copy at
# 	https://www.gnu.org/licenses/lgpl.html)
# ====================================================

# A bunch of constants
module Reedb

	# The version of reedb. This is actually written into vaults to identify
	# breaking changes and using an older sub-set of the API to interact with
	# legacy vaults.
	VERSION = "0.10.rc2"
	NET_PORT = 55736
	TOKEN_BYTE_SIZE = 16 # in bytes
	DEF_MASTER_PATH = "__sysmas__" # Placeholder
	FILE_CACHE_TIME = 2500 # Time in ms
 	KEY_CACHE_TIME = (15 * 60) * 1000 # (30 minutes in ms)



 	#
 	# THIS SECTION WILL BE RESERVED FOR ERROR CODES AS RETURN VALUES
 	#
 	FILE_MALFORMED_DATA_ERROR = 0x11

 	#
 	# VAULT ERROR CODES
 	#
 	VAULT_FILE_NOT_FOUND_ERROR = 0x12


 	#
 	# RETURN ERROR CODES
 	#
 	DAEMON_UNKNOWN_TOKEN_ERROR = 0x31
 	DAEMON_UNAUTHORISED_TOKEN_ERROR = 0x32
 	DAEMON_INACTIVE_VAULT_ERROR = 0x33
end