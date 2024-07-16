@echo off

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8
cmake --install .
