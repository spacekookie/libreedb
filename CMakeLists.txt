cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_BUILD_TYPE Debug)
project(reedb)
add_subdirectory(reedb)

add_executable(reedbd-http
        reedbd-http/main.cpp)

# Library dependencies for the http extention
target_link_libraries(reedbd-http
        reedb
        microhttpd
        protobuf)
