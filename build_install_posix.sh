#!/bin/sh

mkdir build
cd build || exit 1
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j6
sudo cmake --install .
