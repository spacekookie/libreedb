add_executable(reedbd-http main.cpp)

# Library dependencies for the http extention
target_link_libraries(reedbd-http
        reedb
        microhttpd
        protobuf)