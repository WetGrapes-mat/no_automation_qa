#!/bin/zsh
rm -R build
mkdir build 
cd build 
cmake ..
cmake  --build .
./game-04-1
