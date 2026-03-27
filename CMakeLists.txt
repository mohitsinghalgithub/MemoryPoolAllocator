cmake_minimum_required(VERSION 3.10)
project(CustomAllocator CXX)
add_executable(CustomAllocator CustomMemoryPool.cpp
                          FixedPoolTest.cpp
)
