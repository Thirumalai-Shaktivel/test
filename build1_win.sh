#!/usr/bin/env bash
#
# lfortran build script for windows
#
# Example:
# xonsh ./build1_win.sh

bash ci/version.sh
python grammar/asdl_cpp.py
python grammar/asdl_cpp.py src/libasr/ASR.asdl src/libasr/asr.h
pushd src/lfortran/parser && re2c -W -b tokenizer.re -o tokenizer.cpp && popd
pushd src/lfortran/parser && re2c -W -b preprocessor.re -o preprocessor.cpp && popd
pushd src/lfortran/parser && bison -Wall -d -r all parser.yy && popd
