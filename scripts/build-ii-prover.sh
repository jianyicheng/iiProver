#!/bin/bash

. ./bin/env.tcl

# Build LLVM
mkdir -p $IIP/llvm/build
cd $IIP/llvm/build
cmake ../llvm -DLLVM_ENABLE_PROJECTS="llvm" \
    -DLLVM_INSTALL_UTILS=ON -DLLVM_TARGETS_TO_BUILD="X86" \
    -DLLVM_ENABLE_ASSERTIONS=ON \
    -DLLVM_BUILD_EXAMPLES=OFF \
    -DLLVM_ENABLE_RTTI=OFF \
    -DCMAKE_BUILD_TYPE=DEBUG
make -j $(($(grep -c ^processor /proc/cpuinfo) / 2))

# Build II Prover
mkdir -p $IIP/src/build
cd $IIP/src/build
cmake .. \
    -DLLVM_ROOT=../../llvm/build \
    -DCMAKE_BUILD_TYPE=DEBUG
make -j