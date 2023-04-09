#!/bin/zsh
if [[ "$(uname -s)" == "Darwin" ]]; then
    rm -R build
    mkdir build 
    cd build 
    cmake -G "Ninja" .. 
    cmake  --build . 
    ./dynamic-sdl
    echo "====================================="
    if otool -L dynamic-sdl | grep -q "libSDL2"; then
        otool -L dynamic-sdl
        echo "====================================="
        echo "Binary file is linked to SDL2"
        echo "This version is outdated link SDL3"
        install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib /usr/local/lib/libSDL3.1.0.0.dylib dynamic-sdl
        ./dynamic-sdl
        echo "====================================="
        otool -L dynamic-sdl
        du -h dynamic-sdl
        echo "====================================="
    else
        echo "You are using current version"
        otool -L dynamic-sdl
        du -h dynamic-sdl
    fi
else
    rm -R build
    mkdir build 
    cd build 
    cmake .. 
    cmake  --build . 
    du -h dynamic-sdl
fi