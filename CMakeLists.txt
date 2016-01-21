cmake_minimum_required(VERSION 3.3)
project(UThread)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES UThread.c)
add_executable(UThread ${SOURCE_FILES})