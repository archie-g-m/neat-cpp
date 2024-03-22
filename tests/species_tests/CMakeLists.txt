enable_testing()

set(SPECIES_TEST
species_test.cpp 
${PROJECT_SOURCE_DIR}/src/species.cpp
${PROJECT_SOURCE_DIR}/src/genome.cpp
${PROJECT_SOURCE_DIR}/src/config_parser.cpp
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp
${PROJECT_SOURCE_DIR}/src/activations.cpp
${PROJECT_SOURCE_DIR}/src/aggregations.cpp)

add_executable(species_tests ${SPECIES_TEST})
target_link_libraries(species_tests gtest)
target_include_directories(species_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME SpeciesTests COMMAND species_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
