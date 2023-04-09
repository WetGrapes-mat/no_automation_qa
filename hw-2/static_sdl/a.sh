#!/bin/zsh
if [[ "$(uname -s)" == "Darwin" ]]; then
    rm -R build
    mkdir build 
    cd build 
    cmake -G "Ninja" .. 
    cmake  --build . 
    ./static-sdl
    otool -L static-sdl
    du -h static-sdl
else
    rm -R build
    mkdir build 
    cd build 
    cmake .. 
    cmake  --build . 
    du -h static-sdl
fi