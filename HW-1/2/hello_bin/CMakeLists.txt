cmake_minimum_required(VERSION 3.22)
project(hello-bin CXX)

add_executable(hello-bin-static src/main.cxx)
target_compile_features(hello-bin-static PRIVATE cxx_std_20)
target_link_libraries(hello-bin-static LINK_PUBLIC hello-lib-static)

add_executable(hello-bin-dynamic src/main.cxx)
target_compile_features(hello-bin-dynamic PRIVATE cxx_std_20)
target_link_libraries(hello-bin-dynamic LINK_PUBLIC hello-lib-dynamic)

