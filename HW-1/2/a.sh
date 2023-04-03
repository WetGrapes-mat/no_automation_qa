#! /bin/bash
rm -R build
mkdir build 
cd build 
cmake -G "Ninja" .. 
cmake  --build . 
DYNAMIC=`./hello-bin-dynamic`
echo Динамическая: $DYNAMIC
otool -L hello-bin-dynamic 
echo ===========================
STATIC=`./hello-bin-static`
echo Статическая: $STATIC
otool -L hello-bin-static 
