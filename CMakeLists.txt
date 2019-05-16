cmake_minimum_required(VERSION 3.10)
project(lil)

set(CMAKE_CXX_STANDARD 14)

add_library(lil ClientTCP.cpp ClientTCP.h)
add_executable(main main.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

target_link_libraries(main ${CMAKE_SOURCE_DIR}/cmake-build-debug/liblil.a)
IF (WIN32)
    target_link_libraries(main WS2_32.lib)
ENDIF ()
