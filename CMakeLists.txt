cmake_minimum_required(VERSION 2.8)

project(simd-experiment)
find_package(Threads)

include_directories(src)
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -std=c++14 -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopt-info-vec-optimized")

set(SOURCES src/strided-column-simple-correctness.cpp)
set(SOURCES src/strided-column-simd-correctness.cpp)
set(SOURCES src/strided-column-simple.cpp)
set(SOURCES src/strided-column-simd.cpp)

add_executable(strided-column-simple-correctness src/strided-column-simple-correctness.cpp)
add_executable(strided-column-simd-correctness src/strided-column-simd-correctness.cpp)
add_executable(strided-column-simple src/strided-column-simple.cpp)
add_executable(strided-column-simd src/strided-column-simd.cpp)

target_link_libraries(strided-column-simple ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(strided-column-simd ${CMAKE_THREAD_LIBS_INIT})