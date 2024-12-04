cmake_minimum_required(VERSION 3.29)
project(Plant_CPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(Plant_CPP
        Plant.h
        Plant.cpp
        Plant_test.cpp)
