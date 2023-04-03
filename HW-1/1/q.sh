#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake .. 
cmake  --build . 
ctest
./hello_world