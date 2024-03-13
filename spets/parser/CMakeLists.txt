# CMakeLists.txt
add_library(parsercommon SHARED parser.cpp)
add_library(parsing_helpers SHARED parsing_helpers.cpp)

target_include_directories(parsercommon PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(parsing_helpers PRIVATE ${CMAKE_SOURCE_DIR}/include)
