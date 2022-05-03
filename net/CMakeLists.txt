cmake_minimum_required(VERSION 3.11.2)
project(demo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(base)
include_directories(net)

add_executable(demo
                base/TimeStamp.cpp)       
