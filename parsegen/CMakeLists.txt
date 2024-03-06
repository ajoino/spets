# CMakeLists.txt
cmake_minimum_required(VERSION 3.28)

find_library(PARSER_COMMON parsercommon HINT ${CMAKE_SOURCE_DIR}/build/parser)
find_library(LEXER lexer HINT ${CMAKE_SOURCE_DIR}/build/tokenizer)
project(spets_parsegen CXX)

add_executable(parsegen pgen_guido.cpp grammar_parser.cpp)

target_link_libraries(parsegen PRIVATE ${PARSER_COMMON} ${LEXER})
target_include_directories(parsegen PRIVATE ${CMAKE_SOURCE_DIR}/include)
set_target_properties(parsegen PROPERTIES EXCLUDE_FROM_ALL TRUE)
