add_subdirectory(tokenizer)
add_subdirectory(parser)
add_subdirectory(parsegen)


add_executable(spetsc spetsc.cpp)
target_include_directories(spetsc PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(spetsc lexer)
target_link_libraries(spetsc lexer)

