enable_testing()

set(POPULATION_TEST
population_test.cpp 
${PROJECT_SOURCE_DIR}/src/population.cpp
${PROJECT_SOURCE_DIR}/src/species.cpp
${PROJECT_SOURCE_DIR}/src/genome.cpp
${PROJECT_SOURCE_DIR}/src/config_parser.cpp
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp
${PROJECT_SOURCE_DIR}/src/activations.cpp
${PROJECT_SOURCE_DIR}/src/aggregations.cpp)

add_executable(population_tests ${POPULATION_TEST})
target_link_libraries(population_tests gtest)
target_include_directories(population_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME PopulationTests COMMAND population_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
