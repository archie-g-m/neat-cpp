enable_testing()

set(ATTRIBUTE_TEST
aggregation_tests.cpp 
${PROJECT_SOURCE_DIR}/src/aggregations.cpp)

add_executable(aggregation_tests ${ATTRIBUTE_TEST})
target_link_libraries(aggregation_tests gtest)
target_include_directories(aggregation_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME AggregationTests COMMAND aggregation_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})