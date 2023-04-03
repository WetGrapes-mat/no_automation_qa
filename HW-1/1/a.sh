#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake -G "Ninja" .. 
cmake  --build . 
ctest
./hello_world