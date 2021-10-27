#!/bin/bash

set -e
set -x

make -j16 wasm_demo

# Use -Oz for small size

cd src/bin
em++ -Wall -Wextra -fexceptions @CMakeFiles/wasm_demo.dir/objects1.rsp -o wasm_demo.html @CMakeFiles/wasm_demo.dir/linklibs.rsp -s EXPORTED_RUNTIME_METHODS=ccall --shell-file shell_minimal.html
