cmake_minimum_required(VERSION 3.10)
project(Lfu_Cache)

add_executable(cache cache.cpp)

set(INCLUDE_DIR ./include/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -O2 -Wall") # use CMAKE_CXX_STANDARD
set(CMAKE_CXX_STANDARD 20)

target_include_directories(cache PUBLIC ${INCLUDE_DIR})