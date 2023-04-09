cmake_minimum_required(VERSION 3.26)
project(hello-lib)

# select SHARED or STATIC library
add_library(hello_lib_static STATIC include/hello_lib.hxx src/hello_lib.cxx)
target_include_directories(hello_lib_static
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello_lib_static PUBLIC cxx_std_20)

add_library(hello_lib_dynamic SHARED include/hello_lib.hxx src/hello_lib.cxx)
target_include_directories(hello_lib_dynamic
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello_lib_dynamic PUBLIC cxx_std_20)
