#!/bin/bash 
rm -rf CMakeCache.txt 

git submodule init
git submodule update -recursive

mkdir OpenMesh/Build
cd OpenMesh/Build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd -

cmake .
make 
