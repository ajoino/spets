# CMakeLists.txt
add_library(rule rule.cpp)
target_include_directories(rule PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(parsegen pgen_guido.cpp grammar_parser.cpp)
target_include_directories(parsegen PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(parsegen PRIVATE parsercommon lexer rule parsing_helpers -fsanitize=address)
set_target_properties(parsegen PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_custom_command(
    TARGET parsegen
    POST_BUILD
    COMMAND parsegen ARGS Meta
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metagrammar.peg
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metaparser_.cpp
        ${CMAKE_SOURCE_DIR}/include/parsegen/metaparser_.hpp
    BYPRODUCTS
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metaparser_.cpp
        ${CMAKE_SOURCE_DIR}/include/parsegen/metaparser_.hpp
    COMMENT "Run parser generator."
)

add_executable(metaparser metaparser_generator.cpp metaparser.cpp)
target_include_directories(metaparser PRIVATE ${CMAKE_SOURCE_DIR}/include -fsanitize=address)
target_link_libraries(metaparser PUBLIC parsercommon lexer rule parsing_helpers)
set_target_properties(metaparser PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_custom_command(
    TARGET metaparser
    POST_BUILD
    COMMAND metaparser ARGS Meta
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metagrammar.peg
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metaparser_candidate.cpp
        ${CMAKE_SOURCE_DIR}/include/parsegen/metaparser_candidate.hpp
    BYPRODUCTS
        ${CMAKE_SOURCE_DIR}/parsegen/metaparser_candidate.cpp
        ${CMAKE_SOURCE_DIR}/include/parsegen/metaparser_candidate.hpp
    COMMENT "Generate metaparser source files"
    DEPENDS metaparser
)

add_executable(metaparser_runner metaparser_runner.cpp metaparser_candidate.cpp)
target_include_directories(metaparser_runner PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(metaparser_runner PRIVATE parsercommon lexer rule parsing_helpers)
target_compile_definitions(metaparser_runner PRIVATE METAPARSER_CANDIDATE=1)
set_target_properties(metaparser_runner PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_custom_command(
    TARGET metaparser_runner
    POST_BUILD
    COMMAND metaparser_runner ARGS
        ${CMAKE_SOURCE_DIR}/spets/parsegen/metagrammar.peg
    COMMENT "Run metaparser candidate."
)


add_executable(token_inspector token_inspector.cpp)
target_include_directories(token_inspector PRIVATE ${CMAKE_SOURCE_DIR}/include -fsanitize=address)
target_link_libraries(token_inspector PRIVATE lexer)

