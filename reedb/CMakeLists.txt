# Define a library of the following source files. Headers will be automatically loaded
add_library (reedb

  # Utilities
  src/utils/helper.c
  src/utils/uuid.c
  
  # Core interface
  src/core.cpp
  src/vaults.cpp
  
  # Crypto
  
  # Data storage
  src/ree_vault.cpp
  # src/datafile.cpp
)

# Link libraries against us
target_link_libraries(reedb
    cryptopp 	# Main C++ crypto library
    gcrypt   	# Additional crypto library
)

# Include the subdirectories to search for headers
target_include_directories (reedb PUBLIC "include")
target_include_directories (reedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties (reedb PROPERTIES PREFIX "")

  # Crypto stuff
  # src/crypto/token.c
  # src/crypto/hashes.c
  # src/crypto/engine_wrap.c
  # src/crypto/RCryEngine.cpp

  # Utilities
  # src/utils/hashmap.c
  # src/utils/files.c

  # Public interface
  # src/core_legacy.c
  # src/uuid.c
  # src/vault.c

  # Data storage
  # src/datafile.c
  # src/ree_vault.c