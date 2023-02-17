cmake_minimum_required(VERSION 3.20)
project(MemPool)

set(CMAKE_CXX_STANDARD 20)

add_executable(MemPool main.cpp MemPool.hpp FixedHeap.hpp)
