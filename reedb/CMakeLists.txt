
add_library (reedb
	# Crypto stuff
	src/crypto/token.c

	# Utilities
	src/utils/hashmap.c

	# Public interface
	src/core.c
	src/uuid.c
	src/vault.c

	# Core stuff
	src/datafile.c
)

target_include_directories (reedb PUBLIC "include")
target_include_directories (reedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties (reedb PROPERTIES PREFIX "")


