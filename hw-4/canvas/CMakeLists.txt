cmake_minimum_required(VERSION 3.26)
project(sdl-engine-hot-reload)

project(canvas)

add_executable(canvas canvas.cxx canvas.hxx main.cxx)

target_compile_features(canvas PUBLIC cxx_std_20)