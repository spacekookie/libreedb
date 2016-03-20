cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

# Set our source file scope for the library
set(REEDB_SOURCE

        # Public C++ interfaces
        src/core.cpp
        src/vaults.cpp
        #src/config.cpp

        # Public C interfaces
        # src/core.c
        # src/vaults.c
        # src/config.c

        # Utilities
        src/utils/helper.c
        src/utils/files.c
        src/utils/uuid.cpp

        # Crypto
        src/crypto/vault_token.cpp
        src/crypto/rcry_utils.cpp
        src/crypto/rcry_engine.cpp
        src/crypto/rcry_context.h

        # Data storage
        src/ree_vault.cpp
        src/datafile.cpp

        # Protocol buffer auto generated
        src/datafile.pb.cc
        src/crypto/rcry_context.cpp src/crypto/rcry_token.cpp src/crypto/rcry_token.h include/reedb/utils/prettyprint.h)

# Define our library in cmake
add_library(reedb ${REEDB_SOURCE})

# Dynamically linked libraries
target_link_libraries(reedb

        # Boost dependencies
        boost_serialization
        boost_filesystem
        boost_system

        # Crypto dependencies
        cryptopp
        gcrypt

        # Utils
        protobuf
        config++
        )

# Include the subdirectories to search for headers
target_include_directories(reedb PUBLIC "include")
target_include_directories(reedb PUBLIC "src")

# since the name starts with 'lib' dont add it again
set_target_properties(reedb PROPERTIES PREFIX "")