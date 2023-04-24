cmake_minimum_required(VERSION 3.26)
set(CMAKE_BUILD_TYPE Debug)
project(sdl-engine)

find_package(SDL3 REQUIRED)

add_library(sdl-engine-lib SHARED engine.cxx engine.hxx)
target_compile_features(sdl-engine-lib PUBLIC cxx_std_20)

target_link_libraries(sdl-engine-lib PRIVATE SDL3::SDL3-shared)

add_executable(sdl-engine main.cxx)
target_compile_features(sdl-engine PUBLIC cxx_std_20)

target_link_libraries(sdl-engine PRIVATE sdl-engine-lib)
