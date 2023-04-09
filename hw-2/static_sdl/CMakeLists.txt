cmake_minimum_required(VERSION 3.26)
project(static-sdl)

add_executable(${PROJECT_NAME} main.cxx)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARIES}-static)
