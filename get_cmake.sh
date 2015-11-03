set -e  # exit on error.
set -x  # echo commands.

CONV_DIR=$PWD && cd ..

# Preparation
if [ ! -f $PWD/my_cmake/bin/cmake ]; then
	wget --no-check-certificate https://cmake.org/files/v3.3/cmake-3.3.2-Linux-x86_64.tar.gz -O cmake.tar.gz
	mkdir my_cmake && tar xf cmake.tar.gz -C my_cmake --strip 1
	alias cmake=$PWD/my_cmake/bin/cmake
fi

cd $CONV_DIR
