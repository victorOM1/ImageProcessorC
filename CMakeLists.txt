cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Werror -pedantic-errors")

add_executable(untitled main.c)
