#!/bin/bash

mkdir -p build &&
cd build &&
cmake -GNinja -DCMAKE_CXX_COMPILER=clang++-16 .. &&
ninja -v
