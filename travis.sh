#!/usr/bin/env bash

build_cmake() {
  CFLAGS="-fsanitize=undefined"
  mkdir $PWD/_build && cd $PWD/_build
  cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/../_install
  cmake --build . --target install
  ctest -V .
}

set -e  # exit on error.
set -x  # echo commands.

eval "build_cmake"