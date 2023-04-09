#! /bin/bash
if [[ "$(uname -s)" == "Darwin" ]]; then
    rm -R build
    mkdir build 
    cd build 
    cmake -G "Ninja" .. 
    cmake  --build . 
    ctest
    echo ===========================
    DYNAMIC=`./hello_dynamic`
    echo Dynamic: $DYNAMIC
    otool -L hello_dynamic 
    echo ===========================
    STATIC=`./hello_static`
    echo Static: $STATIC
    otool -L hello_static 
else
    rm -R build
    mkdir build 
    cd build 
    cmake .. 
    cmake  --build . 
    ctest
    echo ===========================
    DYNAMIC=`./hello_dynamic`
    echo Dynamic: $DYNAMIC
    ldd hello_dynamic 
    echo ===========================
    STATIC=`./hello_static`
    echo Static: $STATIC
    ldd hello_static 
fi