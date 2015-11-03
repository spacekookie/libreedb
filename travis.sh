#!/usr/bin/env bash

set -e  # exit on error.
set -x  # echo commands.

CFLAGS="-fsanitize=undefined"
mkdir $PWD/_build && cd $PWD/_build
cmake ..
cmake --build .

# Clean up
cd $PWD/.. && rm -rf $PWD/_build

echo '  ######################'
echo 'Build passed successfully!'
echo '  ######################'
