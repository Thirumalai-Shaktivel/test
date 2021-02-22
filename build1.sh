#!/bin/bash

set -e
set -x

install_path="inst"

if [ ! -z "$1" ]
then
    install_path="$1"
fi

echo $install_path

cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DWITH_LLVM=yes \
    -DLFORTRAN_BUILD_ALL=yes \
    -DWITH_STACKTRACE=yes \
    -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH_LFORTRAN;$CONDA_PREFIX" \
    -DCMAKE_INSTALL_PREFIX=$install_path \
    .
cmake --build . --target install
