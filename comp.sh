#!/bin/bash

set -ex

lfortran --show-llvm integration_tests/reserved_02.f90 > reserved_02.ll
llc -mtriple=x86_64-apple-darwin19.5.0 -filetype=obj reserved_02.ll
gcc -o a.out reserved_02.o -L/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -Wl,-rpath,/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -llfortran_runtime_static -lm
./a.out

lfortran -c integration_tests/reserved_02.f90 -o reserved_02b.o
gcc -o a2.out reserved_02b.o -L/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -Wl,-rpath,/Users/ondrej/repos/lfortran/lfortran/src/bin/../runtime -llfortran_runtime_static -lm
./a2.out
