enable_testing()

set(NODE_GENE_TEST
node_gene_tests.cpp 
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp)

add_executable(node_gene_tests ${NODE_GENE_TEST})
target_link_libraries(node_gene_tests gtest)
target_include_directories(node_gene_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME NodeGeneTests COMMAND node_gene_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

set(CONNECTION_GENE_TEST
connection_gene_tests.cpp 
${PROJECT_SOURCE_DIR}/src/genes.cpp
${PROJECT_SOURCE_DIR}/src/attributes.cpp)

add_executable(connection_gene_tests ${CONNECTION_GENE_TEST})
target_link_libraries(connection_gene_tests gtest)
target_include_directories(connection_gene_tests PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_test(NAME ConnectionGeneTests COMMAND connection_gene_tests WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
