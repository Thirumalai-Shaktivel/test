#!/usr/bin/env bash

set -e
set -x

# Use -Oz for small size
emcmake cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_FLAGS_DEBUG="-Wall -Wextra -fexceptions" \
    -DWITH_LLVM=no \
    -DLFORTRAN_BUILD_ALL=yes \
    -DWITH_STACKTRACE=no \
    -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH_LFORTRAN;$CONDA_PREFIX" \
    -DCMAKE_INSTALL_PREFIX=`pwd`/inst \
    -DCMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS=yes \
    -DCMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS=yes \
    -DCMAKE_C_RESPONSE_FILE_LINK_FLAG=@ \
    -DCMAKE_CXX_RESPONSE_FILE_LINK_FLAG=@ \
# cmake --build . -j16
