#!/bin/bash
make clean
rm -rf bin/Release/omnidome.app bin/Release/omnidome.dmg
cmake . -DCMAKE_BUILD_TYPE=Release
make -j8 && make deploy
