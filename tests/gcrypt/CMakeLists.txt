cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_BUILD_TYPE Debug)
add_executable(reedb_gcrypt_test 
	gcrypt_test.c)

# Library dependencies for the http extention
target_link_libraries(reedb_gcrypt_test 
	gcrypt)
