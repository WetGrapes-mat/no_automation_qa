cmake_minimum_required(VERSION 3.16)

project(hello)

add_subdirectory(hello_lib)

add_executable(hello_static src/main.cxx)
target_compile_features(hello_static PRIVATE cxx_std_20)
target_link_libraries(hello_static LINK_PUBLIC hello_lib_static)

add_executable(hello_dynamic src/main.cxx)
target_compile_features(hello_dynamic PRIVATE cxx_std_20)
target_link_libraries(hello_dynamic LINK_PUBLIC hello_lib_dynamic)

include(CTest)

add_test(NAME check_hello_static COMMAND hello_static)
add_test(NAME check_hello_dynamic COMMAND hello_dynamic)