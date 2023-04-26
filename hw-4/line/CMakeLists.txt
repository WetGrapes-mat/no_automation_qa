cmake_minimum_required(VERSION 3.26)
project(lines)

add_executable(
  lines
  main.cxx
  ../irender/irender.hxx
  ../irender/irender.cxx
  ../canvas/canvas.hxx
  ../canvas/canvas.cxx
  line.hxx
  line.cxx)

target_include_directories(lines PUBLIC ../canvas ../irender)
target_compile_features(lines PUBLIC cxx_std_20)
