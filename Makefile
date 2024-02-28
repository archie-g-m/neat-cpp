CORES = 1

.PHONY : all
all: setup build

.PHONY : test
test: setup_test build
	cd build && ctest -j${CORES} && cd -

.PHONY : build
build:
	cmake --build build  -j${CORES}

.PHONY : setup
setup:
	cmake -S . -B build

.PHONY : setup_test
setup_test: 
	cmake -S . -B build -DTEST:=ON

.PHONY : clean
clean:
	rm -rf build