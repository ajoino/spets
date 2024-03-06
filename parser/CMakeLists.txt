# CMakeLists.txt
cmake_minimum_required(VERSION 3.28)

project(spets_parser CXX)

add_library(parsercommon SHARED parser.cpp)

target_include_directories(parsercommon PRIVATE ${CMAKE_SOURCE_DIR}/include)
