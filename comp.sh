#!/bin/bash

set -ex

lfortran --show-llvm integration_tests/reserved_02.f90 > reserved_02.ll
llc -filetype=obj reserved_02.ll
clang -o a.out reserved_02.o -L/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -Wl,-no_pie -Wl,-rpath,/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -llfortran_runtime_static -lm
