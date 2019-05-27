cmake_minimum_required(VERSION 3.10)
project(lil)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)

add_executable(server server.cpp)
add_executable(client client.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

target_link_libraries(server ${CMAKE_SOURCE_DIR}/cmake-build-debug/liblil.a)
target_link_libraries(client ${CMAKE_SOURCE_DIR}/cmake-build-debug/liblil.a)

IF (WIN32)
    target_link_libraries(server WS2_32.lib)
    target_link_libraries(client WS2_32.lib)
ENDIF ()