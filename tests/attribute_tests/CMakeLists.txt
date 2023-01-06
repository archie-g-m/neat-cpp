enable_testing()

set(ATTRIBUTE_TEST
attribute_tests.cpp 
${PROJECT_SOURCE_DIR}/src/attributes.cpp)

add_executable(attribute_tests ${ATTRIBUTE_TEST})
target_link_libraries(attribute_tests gtest)
target_include_directories(attribute_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME AttributeTests COMMAND attribute_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})