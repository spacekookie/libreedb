
add_library (reedb
	src/crypto/token.c
	src/core.c
	src/uuid.c
	src/vault.c)

target_include_directories (reedb PUBLIC "include")
target_include_directories (reedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties (reedb PROPERTIES PREFIX "")
