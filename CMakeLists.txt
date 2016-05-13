cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

project(reedb)
add_subdirectory(reedb)

set(DAEMON_SOURCE reedbd-http/main.cpp
                  reedbd-http/addressbook.pb.cc)
add_executable(reedbd-http ${DAEMON_SOURCE})

# Library dependencies for the http extention
target_link_libraries(reedbd-http reedb protobuf)