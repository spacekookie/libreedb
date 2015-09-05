cmake_minimum_required (VERSION 2.8.11)

project (reedb)

add_subdirectory (reedb)

add_executable (reedbd-http
	reedbd-http/test.c)

target_link_libraries(reedbd-http
	libreedb)
