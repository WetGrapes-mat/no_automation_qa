#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake -G "Ninja" .. 
cmake  --build . 
ctest
echo ===========================
DYNAMIC=`./hello_dynamic`
echo Динамическая: $DYNAMIC
otool -L hello_dynamic 
echo ===========================
STATIC=`./hello_static`
echo Статическая: $STATIC
otool -L hello_static 
