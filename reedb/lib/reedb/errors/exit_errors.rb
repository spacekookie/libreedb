module Reedb

	# Exit codes to be used throughout the API
	EXIT_STILL_LOCKED = 0x10
	EXIT_OS_PARSE = 0x11

	# Severe panic codes
	EXIT_PANIC_INTERUPT = 0x31
	EXIT_MISSING_USER_CODE = 0x32
	EXIT_CORRUPT_FS = 0x33
	EXIT_HTTP_MALFUNCT = 0x34
end