# CMakeLists.txt
add_library(parsercommon SHARED parser.cpp)

target_include_directories(parsercommon PRIVATE ${CMAKE_SOURCE_DIR}/include)
