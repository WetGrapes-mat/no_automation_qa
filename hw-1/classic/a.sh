#! /bin/bash
if [[ "$(uname -s)" == "Darwin" ]]; then
    rm -R build
    mkdir build 
    cd build 
    cmake -G "Ninja" .. 
    cmake  --build . 
    ctest
    ./hello_world
else
    rm -R build
    mkdir build 
    cd build 
    cmake .. 
    cmake  --build . 
    ctest
    ./hello_world
fi