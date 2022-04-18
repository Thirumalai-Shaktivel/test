#!/bin/bash

set -e
set -x

make -j16 wasm_demo

# Use -Oz for small size

# cd src/bin
em++ -Wall -Wextra -fexceptions @CMakeFiles/wasm_demo.dir/objects1.rsp -o wasm_demo.html @CMakeFiles/wasm_demo.dir/linklibs.rsp -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' --shell-file shell_minimal.html
# em++ -Wall -Wextra -fexceptions wasm_demo -o wasm_demo.html -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' --shell-file shell_minimal.html
# emcc -Wall -Wextra -fexceptions @CMakeFiles/wasm_demo.dir/objects1.rsp -o wasm_demo.html -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' --shell-file shell_minimal.html
