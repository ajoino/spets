# CmakeLists.txt
cmake_minimum_required(VERSION 3.28)

project(spets)

# set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER clang++-17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -stdlib=libc++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.hpp)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(spetsc spets/spetsc.cpp parsegen/lexer.cpp)
target_include_directories(spetsc PRIVATE ${PROJECT_SOURCE_DIR})

add_executable(pgen parsegen/pgen_guido.cpp parsegen/peg_parser.cpp parsegen/lexer.cpp)
target_include_directories(pgen PRIVATE ${PROJECT_SOURCE_DIR})

add_executable(parser parsegen/generated_parser.cpp parsegen/lexer.cpp)
target_include_directories(parser PRIVATE ${PROJECT_SOURCE_DIR})

add_custom_target(build_parser
    COMMAND pgen parsegen/test_grammar.peg
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Run parser generator."
    SOURCES ${PROJECT_SOURCE_DIR}
)

add_dependencies(parser build_parser)
