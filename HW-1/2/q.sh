#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake .. 
cmake  --build . 
ctest
echo ===========================
DYNAMIC=`./hello_dynamic`
echo Динамическая: $DYNAMIC
ldd hello_dynamic 
echo ===========================
STATIC=`./hello_static`
echo Статическая: $STATIC
ldd hello_static 