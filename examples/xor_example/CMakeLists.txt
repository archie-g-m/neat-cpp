cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(xor-example
VERSION 1.0.0
DESCRIPTION "XOR example of NEAT Algorithm in C++"
LANGUAGES CXX
)

set(SOURCES
xor_example.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
PUBLIC neat-cpp)
