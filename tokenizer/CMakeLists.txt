# CMakeLists.txt
cmake_minimum_required(VERSION 3.28)

project(spets_tokenizer CXX)

# set(CMAKE_CXX_COMPILER clang++-17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -stdlib=libc++ -g")
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(lexer SHARED lexer.cpp)
# target_sources(lexer PRIVATE lexer.cpp)
target_include_directories(lexer PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_compile_features(lexer PRIVATE cxx_std_17)
