cmake_minimum_required (VERSION 2.8.11)

project (reedb)

add_subdirectory (libreedb)

add_executable (reedbd
	src/test.c)

target_link_libraries(reedbd
	libreedb)
