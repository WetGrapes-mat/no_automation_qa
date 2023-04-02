#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake -G "Ninja" .. 
cmake  --build . 
cd hello_bin
echo Динамическая:
otool -L hello-bin-dynamic 
./hello-bin-dynamic
echo ===========================
echo Статическая:
otool -L hello-bin-static 
./hello-bin-static
