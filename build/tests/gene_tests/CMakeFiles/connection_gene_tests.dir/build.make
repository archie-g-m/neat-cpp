# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/archiemilligan/Documents/NEAT-cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/archiemilligan/Documents/NEAT-cpp/build

# Include any dependencies generated for this target.
include tests/gene_tests/CMakeFiles/connection_gene_tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/gene_tests/CMakeFiles/connection_gene_tests.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/gene_tests/CMakeFiles/connection_gene_tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/gene_tests/CMakeFiles/connection_gene_tests.dir/flags.make

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/flags.make
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o: /Users/archiemilligan/Documents/NEAT-cpp/tests/gene_tests/connection_gene_tests.cpp
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/archiemilligan/Documents/NEAT-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o -MF CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o.d -o CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o -c /Users/archiemilligan/Documents/NEAT-cpp/tests/gene_tests/connection_gene_tests.cpp

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.i"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/archiemilligan/Documents/NEAT-cpp/tests/gene_tests/connection_gene_tests.cpp > CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.i

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.s"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/archiemilligan/Documents/NEAT-cpp/tests/gene_tests/connection_gene_tests.cpp -o CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.s

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/flags.make
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o: /Users/archiemilligan/Documents/NEAT-cpp/src/genes.cpp
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/archiemilligan/Documents/NEAT-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o -MF CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o.d -o CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o -c /Users/archiemilligan/Documents/NEAT-cpp/src/genes.cpp

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.i"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/archiemilligan/Documents/NEAT-cpp/src/genes.cpp > CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.i

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.s"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/archiemilligan/Documents/NEAT-cpp/src/genes.cpp -o CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.s

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/flags.make
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o: /Users/archiemilligan/Documents/NEAT-cpp/src/attributes.cpp
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/archiemilligan/Documents/NEAT-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o -MF CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o.d -o CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o -c /Users/archiemilligan/Documents/NEAT-cpp/src/attributes.cpp

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.i"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/archiemilligan/Documents/NEAT-cpp/src/attributes.cpp > CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.i

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.s"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/archiemilligan/Documents/NEAT-cpp/src/attributes.cpp -o CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.s

# Object files for target connection_gene_tests
connection_gene_tests_OBJECTS = \
"CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o" \
"CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o" \
"CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o"

# External object files for target connection_gene_tests
connection_gene_tests_EXTERNAL_OBJECTS =

tests/gene_tests/connection_gene_tests: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/connection_gene_tests.cpp.o
tests/gene_tests/connection_gene_tests: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/genes.cpp.o
tests/gene_tests/connection_gene_tests: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/__/__/src/attributes.cpp.o
tests/gene_tests/connection_gene_tests: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/build.make
tests/gene_tests/connection_gene_tests: lib/libgtest.a
tests/gene_tests/connection_gene_tests: tests/gene_tests/CMakeFiles/connection_gene_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/archiemilligan/Documents/NEAT-cpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable connection_gene_tests"
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/connection_gene_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/gene_tests/CMakeFiles/connection_gene_tests.dir/build: tests/gene_tests/connection_gene_tests
.PHONY : tests/gene_tests/CMakeFiles/connection_gene_tests.dir/build

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/clean:
	cd /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests && $(CMAKE_COMMAND) -P CMakeFiles/connection_gene_tests.dir/cmake_clean.cmake
.PHONY : tests/gene_tests/CMakeFiles/connection_gene_tests.dir/clean

tests/gene_tests/CMakeFiles/connection_gene_tests.dir/depend:
	cd /Users/archiemilligan/Documents/NEAT-cpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/archiemilligan/Documents/NEAT-cpp /Users/archiemilligan/Documents/NEAT-cpp/tests/gene_tests /Users/archiemilligan/Documents/NEAT-cpp/build /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests /Users/archiemilligan/Documents/NEAT-cpp/build/tests/gene_tests/CMakeFiles/connection_gene_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/gene_tests/CMakeFiles/connection_gene_tests.dir/depend

