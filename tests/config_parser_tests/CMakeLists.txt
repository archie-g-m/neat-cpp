enable_testing()

set(CONFIG_PARSER_TEST
config_parser_tests.cpp 
${PROJECT_SOURCE_DIR}/src/config_parser.cpp)

add_executable(config_parser_tests ${CONFIG_PARSER_TEST})
target_link_libraries(config_parser_tests gtest)
target_include_directories(config_parser_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME ConfigParserTests COMMAND config_parser_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
