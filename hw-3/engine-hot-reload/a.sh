#!/bin/zsh
rm -R build
mkdir build 
cd build 
cmake ..
cmake  --build .
# ./sdl-engine
./engine
