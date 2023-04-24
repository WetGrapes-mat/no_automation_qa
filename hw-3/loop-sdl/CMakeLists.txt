cmake_minimum_required(VERSION 3.26)
project(sdl-loop)

add_executable(sdl-loop main.cxx)
target_compile_features(sdl-loop PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)

target_link_libraries(sdl-loop PRIVATE SDL3::SDL3-shared)
