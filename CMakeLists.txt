
cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TEST "Enable Testing" OFF)

project(neat-cpp
VERSION 1.0.0
DESCRIPTION "NEAT Algorithm in C++"
)



set(SOURCES
src/main.cpp
src/activations.cpp
src/aggregations.cpp
src/attributes.cpp
src/genes.cpp
src/genome.cpp
src/config_parser.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

if(${TEST})
    enable_testing()
    add_subdirectory(lib/googletest)
    add_subdirectory(tests)
endif()











