set(SOURCES
main.cpp
activations.cpp
aggregations.cpp
attributes.cpp
genes.cpp
genome.cpp
config_parser.cpp)
add_executable(main ${SOURCES})
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/config)

