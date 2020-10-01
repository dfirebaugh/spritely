#!/bin/sh

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j9
cd ..
