CORES = 1
CMAKE_FLAGS = 

.PHONY : all
all: setup build

.PHONY : examples
examples: setup_examples build

.PHONY : test
test: setup_test build
	cd build && ctest -j${CORES} && cd -

.PHONY : build
build:
	cmake --build build  -j${CORES} 

.PHONY : setup
setup:
	cmake -S . -B build ${CMAKE_FLAGS}

.PHONY : setup_test
setup_test: 
	cmake -S . -B build -DTEST:=ON ${CMAKE_FLAGS}

.PHONY : setup_examples
setup_examples:
	cmake -S . -B build -DEXAMPLES:=ON ${CMAKE_FLAGS}

.PHONY : clean
clean:
	rm -rf build