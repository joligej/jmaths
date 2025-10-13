#!/bin/sh

mkdir -p build
cd build || exit 1
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
sudo cmake --install .
