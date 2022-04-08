#!/bin/bash

FILENAME="dijkstra"
tmp="_norm"
output=$FILENAME$tmp
clang -g -O3 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -S -emit-llvm $FILENAME.c -o $FILENAME.ll
opt $FILENAME.ll -S -o $output.ll


cd ../../build
LLVM_BUILD_DIR=/home/rouzbeh/Graduate/Research/LLVM/llvm-project/build
cmake -DCMAKE_VERBOSE_MAKEFILE=ON \
  -DLLVM_DIR=$LLVM_BUILD_DIR/lib/cmake/llvm          \
  -DCMAKE_C_COMPILER=$LLVM_BUILD_DIR/bin/clang       \
  -DCMAKE_CXX_COMPILER=$LLVM_BUILD_DIR/bin/clang++   \
  ../
make 
cd ../test/Compiled_files
opt -load-pass-plugin=../../build/ALC.so -passes="check-alc-conditions"  $output.ll -debug-only=loop-accesses --disable-output
