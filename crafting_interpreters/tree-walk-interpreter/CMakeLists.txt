cmake_minimum_required(VERSION 3.16)

project(lox LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_library(lox_lib STATIC 
    scanner.cpp 
    tokens.cpp 
    lox.cpp 
    syntaxTree.cpp 
    parser.cpp
    interpreter.cpp)
add_executable(lox main.cpp)
target_link_libraries(lox PUBLIC lox_lib)


set_property(TARGET lox PROPERTY CXX_STANDARD 17)
set_property(TARGET lox_lib PROPERTY CXX_STANDARD 17)
