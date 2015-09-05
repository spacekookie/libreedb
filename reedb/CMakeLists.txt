
add_library (libreedb
	src/crypto/token.c
	src/core.c
	src/uuid.c
	src/vault.c)

target_include_directories (libreedb PUBLIC "include")
target_include_directories (libreedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties (libreedb PROPERTIES PREFIX "")
