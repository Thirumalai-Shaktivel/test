#!/bin/bash

set -e
set -x

emcmake cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS_DEBUG="-Wall -Wextra -g -fexceptions" \
    -DWITH_LLVM=no \
    -DLFORTRAN_BUILD_ALL=yes \
    -DWITH_STACKTRACE=no \
    -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH_LFORTRAN;$CONDA_PREFIX" \
    -DCMAKE_INSTALL_PREFIX=`pwd`/inst \
    .
#cmake --build . -j16 --target install
make -j16 wasm_demo
