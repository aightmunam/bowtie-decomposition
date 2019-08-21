cmake_minimum_required(VERSION 3.14)
project(Algorithm)

set(CMAKE_CXX_STANDARD 14)
MATH(EXPR stack_size "16 * 1024 * 1024") #16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
add_executable(Algorithm main.cpp)