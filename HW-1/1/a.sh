#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake -G "Ninja" .. 
cmake  --build . 
./hello_world