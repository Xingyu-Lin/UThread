cmake_minimum_required(VERSION 3.3)
project(UThread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")

set(SOURCE_FILES UThread.cpp test.cpp)
add_executable(UThread ${SOURCE_FILES})