cmake_minimum_required(VERSION 3.26)
project(hello_world)

add_executable(hello_world main.cxx)
target_compile_features(hello_world PUBLIC cxx_std_20)

include(CTest)

add_test(NAME check_hello_exit_zero COMMAND hello_world)
