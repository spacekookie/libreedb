add_library (reedb

	# Crypto stuff
	src/crypto/token.c
	src/crypto/engine.c
	src/crypto/hashes.c

	# Utilities
	src/utils/hashmap.c
	src/utils/files.c

	# Public interface
	src/core.c
	src/uuid.c
	src/vault.c

	# Data storage
	src/datafile.c
	src/ree_vault.c
)

target_link_libraries(reedb
	gcrypt)

target_include_directories (reedb PUBLIC "include")
target_include_directories (reedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties (reedb PROPERTIES PREFIX "")
