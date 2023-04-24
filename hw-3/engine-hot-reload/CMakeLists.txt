cmake_minimum_required(VERSION 3.26)
project(sdl-engine-hot-reload)

add_executable(engine engine.cxx engine.hxx)
target_compile_features(engine PUBLIC cxx_std_20)

set_target_properties(engine PROPERTIES ENABLE_EXPORTS 1)
find_package(SDL3 REQUIRED)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared)

add_library(game-lib SHARED game.cxx)

target_compile_features(game-lib PUBLIC cxx_std_20)
target_link_libraries(game-lib PRIVATE engine)
