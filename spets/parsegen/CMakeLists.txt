# CMakeLists.txt
add_library(rule rule.cpp)
target_include_directories(rule PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(parsegen pgen_guido.cpp grammar_parser.cpp)
add_dependencies(parsegen parsercommon lexer)
target_include_directories(parsegen PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parsegen PRIVATE parsercommon lexer rule parsing_helpers -fsanitize=address)
set_target_properties(parsegen PROPERTIES EXCLUDE_FROM_ALL TRUE)

# add_custom_command(
#     TARGET parsegen
#     POST_BUILD
#     COMMAND ${CMAKE_SOURCE_DIR}/build/spets/parsegen/parsegen ${CMAKE_SOURCE_DIR}/spets/parsegen/test_grammar.peg ${CMAKE_SOURCE_DIR}/spets/parsegen/generated_parser.cpp
#     BYPRODUCTS ${CMAKE_SOURCE_DIR}/parsegen/generated_parser.cpp
#     COMMENT "Run parser generator."
#     DEPENDS parsegen
# )

add_custom_command(
    TARGET parsegen
    POST_BUILD
    COMMAND ${CMAKE_SOURCE_DIR}/build/spets/parsegen/parsegen ${CMAKE_SOURCE_DIR}/spets/parsegen/metagrammar.peg ${CMAKE_SOURCE_DIR}/spets/parsegen/metaparser.cpp
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/parsegen/metaparser.cpp
    COMMENT "Generate metaparser source files"
    DEPENDS parsegen
)

add_executable(parser ${CMAKE_SOURCE_DIR}/spets/parsegen/generated_parser.cpp)
add_dependencies(parser parsercommon lexer parsegen)
target_include_directories(parser PRIVATE ${CMAKE_SOURCE_DIR}/include -fsanitize=address)
target_link_libraries(parser PRIVATE parsercommon lexer)

add_executable(metaparser metaparser.cpp)
add_dependencies(metaparser parsercommon lexer parsegen rule parsing_helpers)
target_include_directories(metaparser PRIVATE ${CMAKE_SOURCE_DIR}/include -fsanitize=address)
target_link_libraries(metaparser PRIVATE parsercommon lexer rule parsing_helpers)
set_target_properties(metaparser PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(token_inspector token_inspector.cpp)
target_include_directories(token_inspector PRIVATE ${CMAKE_SOURCE_DIR}/include -fsanitize=address)
target_link_libraries(token_inspector PRIVATE lexer)

