#!/usr/bin/env bash
#
# Generate files required for the build.
#
# Example:
#
# ./build0.sh

set -e
set -x

# Generate the `version` file
ci/version.sh

# Generate a Fortran AST from AST.asdl (C++)
python grammar/asdl_cpp.py
# Generate a Fortran ASR from ASR.asdl (C++)
python grammar/asdl_cpp.py src/libasr/ASR.asdl src/libasr/asr.h

# Generate the tokenizer and parser
(cd src/lfortran/parser && re2c -W -b tokenizer.re -o tokenizer.cpp)
(cd src/lfortran/parser && re2c -W -b preprocessor.re -o preprocessor.cpp)
(cd src/lfortran/parser && bison -Wall -d -r all parser.yy)

grep -n "'" src/lfortran/parser/parser.yy && echo "Single quote not allowed" && exit 1
echo "OK"
