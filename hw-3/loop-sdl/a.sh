#!/bin/zsh
rm -R build
mkdir build 
cd build 
cmake -G "Ninja"  ..
cmake  --build .
./sdl-loop
