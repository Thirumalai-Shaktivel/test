#!/bin/bash

set -e
set -x

cd src/bin

em++ -Wall -Wextra -g -fexceptions @CMakeFiles/wasm_demo.dir/objects1.rsp -o wasm_demo.html @CMakeFiles/wasm_demo.dir/linklibs.rsp
