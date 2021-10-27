#!/bin/bash

set -e
set -x

make -j16 wasm_demo

cd src/bin
em++ -Wall -Wextra -Oz -fexceptions @CMakeFiles/wasm_demo.dir/objects1.rsp -o wasm_demo.html @CMakeFiles/wasm_demo.dir/linklibs.rsp -s EXPORTED_RUNTIME_METHODS=ccall
