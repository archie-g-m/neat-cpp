
cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TEST "Enable Testing" OFF)

project(neat-cpp
VERSION 1.0.0
DESCRIPTION "NEAT Algorithm in C++"
LANGUAGES CXX
)

set(SOURCES
src/main.cpp
src/activations.cpp
src/aggregations.cpp
src/attributes.cpp
src/genes.cpp
src/genome.cpp
src/config_parser.cpp
src/species.cpp
src/population.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(${TEST})
    add_compile_definitions(TEST_MODE)
    enable_testing()
    add_subdirectory(lib/googletest)
    add_subdirectory(tests)
endif()

if(${EXAMPLES})
    add_subdirectory(examples)
endif()











