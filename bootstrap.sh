#!/bin/sh 

# Bootstrap script for ubuntu and MacOSX for fetching cm8kr and omnicalib

# Clone dependencies from github
cd ..
# clone cm8kr in to path ./cm8kr if doesnt exist yet
if [ ! -d "cm8kr" ]; then
  git clone https://github.com/cr8tr/cm8kr
fi

if [ ! -d "omnicalib" ]; then
  git clone https://github.com/cr8tr/omnicalib
fi
cd -

echo $1

cmake . -DCMAKE_BUILD_TYPE=$1
make
