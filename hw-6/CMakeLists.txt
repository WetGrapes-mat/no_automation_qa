cmake_minimum_required(VERSION 3.26)
project(opengl-minimal CXX C)

list(
  APPEND
  SHADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/triangle_vertex.shader"
  "${CMAKE_CURRENT_SOURCE_DIR}/triangle_fragment.shader"
  "${CMAKE_CURRENT_SOURCE_DIR}/texture_vertex.shader"
  "${CMAKE_CURRENT_SOURCE_DIR}/texture_fragment.shader")
list(APPEND IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/fly.png")
file(COPY ${SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${IMAGES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(engine SHARED engine.cxx engine.hxx glad/glad.c glad/include/glad.h
                          glad/include/KHR/khrplatform.h)
target_compile_features(engine PUBLIC cxx_std_20)
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared OpenGL::GL glm::glm)

add_executable(game game.cxx)
target_compile_features(game PUBLIC cxx_std_20)
target_link_libraries(game PRIVATE engine)
