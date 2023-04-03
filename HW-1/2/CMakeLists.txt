cmake_minimum_required(VERSION 3.16)

project(01-2-hello)

# set(CMAKE_C_COMPILER /usr/bin/g++)
# set(CMAKE_CXX_COMPILER /usr/bin/g++)

add_subdirectory(hello_lib)

add_executable(hello-bin-static src/main.cxx)
target_compile_features(hello-bin-static PRIVATE cxx_std_20)
target_link_libraries(hello-bin-static LINK_PUBLIC hello-lib-static)

add_executable(hello-bin-dynamic src/main.cxx)
target_compile_features(hello-bin-dynamic PRIVATE cxx_std_20)
target_link_libraries(hello-bin-dynamic LINK_PUBLIC hello-lib-dynamic)