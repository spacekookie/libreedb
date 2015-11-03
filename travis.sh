#!/usr/bin/env bash

# ############################################## #
#                                                #
#  This is the reedb build script that compiles  #
#  Reedb for travis on Linux systems.            #
#                                                #
# ############################################## #

set -e  # exit on error.
set -x  # echo commands.

# Preparation
if [ ! -f ./cmake.tar.gz ]; then
	wget --no-check-certificate https://cmake.org/files/v3.3/cmake-3.3.2-Linux-x86_64.tar.gz -O cmake.tar.gz
	mkdir my_cmake && tar xf cmake.tar.gz -C my_cmake
	alias cmake=$PWD/my_cmake/bin/cmake
fi

# Actual build
CFLAGS="-fsanitize=undefined"
mkdir $PWD/_build && cd $PWD/_build
cmake ..
cmake --build .

# Clean up
cd $PWD/.. && rm -rf $PWD/_build
