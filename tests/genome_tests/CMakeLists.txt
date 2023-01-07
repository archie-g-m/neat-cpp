enable_testing()

set(GENOMECONFIG_TEST
genome_config_tests.cpp 
${PROJECT_SOURCE_DIR}/src/genome.cpp
${PROJECT_SOURCE_DIR}/src/config_parser.cpp
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp)

add_executable(genome_config_tests ${GENOMECONFIG_TEST})
target_link_libraries(genome_config_tests gtest)
target_include_directories(genome_config_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME GenomeConfigTests COMMAND genome_config_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

set(GENOME_TEST
genome_tests.cpp 
${PROJECT_SOURCE_DIR}/src/genome.cpp
${PROJECT_SOURCE_DIR}/src/config_parser.cpp
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp)

add_executable(genome_tests ${GENOME_TEST})
target_link_libraries(genome_tests gtest)
target_include_directories(genome_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME GenomeTests COMMAND genome_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
