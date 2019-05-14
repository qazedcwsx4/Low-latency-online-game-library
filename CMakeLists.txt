cmake_minimum_required(VERSION 3.13)
project(lil)

set(CMAKE_CXX_STANDARD 14)

add_library(lil ClientTCP.cpp ClientTCP.h)
add_executable(test main.cpp)
target_link_libraries(test ${CMAKE_SOURCE_DIR}/cmake-build-debug/liblil.a)
target_link_libraries(test WS2_32.lib)
