# cmake_minimum_required(VERSION <specify CMake version here>)
project(SaltyBot C)

set(CMAKE_C_STANDARD 99)

add_executable(SaltyBot main.c main.h)

add_library(SaltyBot libtwirc.so)