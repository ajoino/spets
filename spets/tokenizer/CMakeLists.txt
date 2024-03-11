# CMakeLists.txt

add_library(lexer SHARED lexer.cpp)
# target_sources(lexer PRIVATE lexer.cpp)
target_include_directories(lexer PRIVATE ${CMAKE_SOURCE_DIR}/include)
# target_compile_features(lexer PRIVATE cxx_std_17)
