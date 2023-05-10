cmake_minimum_required(VERSION 3.26)
project(opengl-minimal CXX C)

add_library(engine SHARED engine.cxx engine.hxx glad/glad.c glad/glad.h
                          KHR/khrplatform.h)
target_compile_features(engine PUBLIC cxx_std_20)
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared)

add_executable(game game.cxx)
target_compile_features(game PUBLIC cxx_std_20)
target_link_libraries(game PRIVATE engine)
