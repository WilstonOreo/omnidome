#!/bin/sh
make clean
rm -rf bin/Release
cmake . -DCMAKE_BUILD_TYPE=Release
make -j8 && cpack && dpkg-deb --contents Omnidome-*.deb
