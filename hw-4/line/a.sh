#!/bin/zsh
rm -R build
mkdir build 
cd build 
cmake ..
cmake  --build .
./lines
