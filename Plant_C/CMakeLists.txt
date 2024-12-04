cmake_minimum_required(VERSION 3.29)
project(Plant_C C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Plant_C
    Plant.c
    Plant.h
    Plant_test.c
)
