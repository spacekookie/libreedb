cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_BUILD_TYPE Debug)
project(reedb)
add_subdirectory(reedb)
add_executable(reedbd-http 
	reedbd-http/main.c)

# Library dependencies for the http extention
target_link_libraries(reedbd-http
		reedb
		microhttpd)
