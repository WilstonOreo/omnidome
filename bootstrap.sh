#!/bin/bash 
rm -rf CMakeCache.txt 

git submodule update --init --remote

mkdir OpenMesh/Build
cd OpenMesh/Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd -

cmake .
make 
