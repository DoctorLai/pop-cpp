# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hieunv/Desktop/popc2.8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hieunv/Desktop/popc2.8

# Utility rule file for popcpp_object_main.

# Include the progress variables for this target.
include lib/dynamic/CMakeFiles/popcpp_object_main.dir/progress.make

lib/dynamic/CMakeFiles/popcpp_object_main: lib/dynamic/paroc_objmain.std.o

lib/dynamic/paroc_objmain.std.o: lib/dynamic/paroc_objmain.std.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/hieunv/Desktop/popc2.8/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Compile POP-C++ Object main"
	cd /home/hieunv/Desktop/popc2.8/lib/dynamic && /usr/bin/g++ -c paroc_objmain.std.cc -I/home/hieunv/Desktop/popc2.8/include/dynamic

popcpp_object_main: lib/dynamic/CMakeFiles/popcpp_object_main
popcpp_object_main: lib/dynamic/paroc_objmain.std.o
popcpp_object_main: lib/dynamic/CMakeFiles/popcpp_object_main.dir/build.make
.PHONY : popcpp_object_main

# Rule to build all files generated by this target.
lib/dynamic/CMakeFiles/popcpp_object_main.dir/build: popcpp_object_main
.PHONY : lib/dynamic/CMakeFiles/popcpp_object_main.dir/build

lib/dynamic/CMakeFiles/popcpp_object_main.dir/clean:
	cd /home/hieunv/Desktop/popc2.8/lib/dynamic && $(CMAKE_COMMAND) -P CMakeFiles/popcpp_object_main.dir/cmake_clean.cmake
.PHONY : lib/dynamic/CMakeFiles/popcpp_object_main.dir/clean

lib/dynamic/CMakeFiles/popcpp_object_main.dir/depend:
	cd /home/hieunv/Desktop/popc2.8 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hieunv/Desktop/popc2.8 /home/hieunv/Desktop/popc2.8/lib/dynamic /home/hieunv/Desktop/popc2.8 /home/hieunv/Desktop/popc2.8/lib/dynamic /home/hieunv/Desktop/popc2.8/lib/dynamic/CMakeFiles/popcpp_object_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/dynamic/CMakeFiles/popcpp_object_main.dir/depend
