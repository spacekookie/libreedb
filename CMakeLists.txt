############################################
#
# libreedb uses cmake because it's simple
#   Build instructions can be found in the README
#
############################################

# Set some flags
cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
set(CMAKE_BUILD_TYPE Debug)

# Create our project for further reference
project(reedb)

# Setup protobuf support for reedb
INCLUDE(FindProtobuf)
#FIND_PACKAGE(Protobuf REQUIRED)
#INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
#PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER src/protos/rdb_data.proto)

# Add the libgdsl build path
add_subdirectory(lib/libcdsl)
add_subdirectory(lib/libdyntree)

set(RDB_SRC

        # Core interfaces
        src/reedb.c
        src/vaults.c
        src/error.c
        src/data.c

        # Core functionality

        # Utility and small subroutines
        src/utils/encoding.c
        src/utils/random.c
        src/utils/platform.c
        src/utils/uuid.c)

# Define our library in cmake
add_library(libreedb STATIC ${RDB_SRC})

# Dynamically linked libraries
target_link_libraries(libreedb
        gcrypt
        protobuf-c
        config
        apr-1
        libcdsl
        libdyntree
)

# Include the subdirectories to search for headers
target_include_directories(libreedb PUBLIC "include")
target_include_directories(libreedb PRIVATE "src")

# since the name starts with 'lib' dont add it again
set_target_properties(libreedb PROPERTIES PREFIX "")

################### TESTING CODE BELOW ###################

set(TEST_SRC bin/data_test.c bin/main.c)
add_executable(reedbd ${TEST_SRC})

# Library dependencies for the http extention
target_link_libraries(reedbd libreedb)
