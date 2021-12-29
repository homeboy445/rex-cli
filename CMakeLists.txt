cmake_minimum_required(VERSION 3.9.1)

project(REX)

set(CMAKE_CXX_STANDARD 20)

add_executable(rex main.cpp util/code_helper.cpp 
include/main.h include/code_helper.h include/utility.h)

