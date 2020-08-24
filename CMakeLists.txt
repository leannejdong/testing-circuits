cmake_minimum_required(VERSION 3.16)
project(tension)

set(CMAKE_CXX_STANDARD 20)

add_executable(tension tension_test.cpp tension.cpp)