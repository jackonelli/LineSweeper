cmake_minimum_required(VERSION 3.10.2)

project(Linesweeper VERSION 0.3 LANGUAGES CXX)

add_compile_options(-std=c++14 -ggdb)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE "Release")

add_subdirectory(src)
