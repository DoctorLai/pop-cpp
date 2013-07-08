# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hieunv/Desktop/popc2.8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hieunv/Desktop/popc2.8

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: install/strip
.PHONY : install/strip/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/hieunv/Desktop/popc2.8/CMakeFiles /home/hieunv/Desktop/popc2.8/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/hieunv/Desktop/popc2.8/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named distclean

# Build rule for target.
distclean: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 distclean
.PHONY : distclean

# fast build rule for target.
distclean/fast:
	$(MAKE) -f CMakeFiles/distclean.dir/build.make CMakeFiles/distclean.dir/build
.PHONY : distclean/fast

#=============================================================================
# Target rules for targets named popc_common

# Build rule for target.
popc_common: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_common
.PHONY : popc_common

# fast build rule for target.
popc_common/fast:
	$(MAKE) -f lib/dynamic/CMakeFiles/popc_common.dir/build.make lib/dynamic/CMakeFiles/popc_common.dir/build
.PHONY : popc_common/fast

#=============================================================================
# Target rules for targets named popcpp_master_main

# Build rule for target.
popcpp_master_main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcpp_master_main
.PHONY : popcpp_master_main

# fast build rule for target.
popcpp_master_main/fast:
	$(MAKE) -f lib/dynamic/CMakeFiles/popcpp_master_main.dir/build.make lib/dynamic/CMakeFiles/popcpp_master_main.dir/build
.PHONY : popcpp_master_main/fast

#=============================================================================
# Target rules for targets named popcpp_object_main

# Build rule for target.
popcpp_object_main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcpp_object_main
.PHONY : popcpp_object_main

# fast build rule for target.
popcpp_object_main/fast:
	$(MAKE) -f lib/dynamic/CMakeFiles/popcpp_object_main.dir/build.make lib/dynamic/CMakeFiles/popcpp_object_main.dir/build
.PHONY : popcpp_object_main/fast

#=============================================================================
# Target rules for targets named popc_common_psdyn

# Build rule for target.
popc_common_psdyn: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_common_psdyn
.PHONY : popc_common_psdyn

# fast build rule for target.
popc_common_psdyn/fast:
	$(MAKE) -f lib/pseudodynamic/CMakeFiles/popc_common_psdyn.dir/build.make lib/pseudodynamic/CMakeFiles/popc_common_psdyn.dir/build
.PHONY : popc_common_psdyn/fast

#=============================================================================
# Target rules for targets named popcpp_master_psdyn_main

# Build rule for target.
popcpp_master_psdyn_main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcpp_master_psdyn_main
.PHONY : popcpp_master_psdyn_main

# fast build rule for target.
popcpp_master_psdyn_main/fast:
	$(MAKE) -f lib/pseudodynamic/CMakeFiles/popcpp_master_psdyn_main.dir/build.make lib/pseudodynamic/CMakeFiles/popcpp_master_psdyn_main.dir/build
.PHONY : popcpp_master_psdyn_main/fast

#=============================================================================
# Target rules for targets named popcpp_object_psdyn_main

# Build rule for target.
popcpp_object_psdyn_main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcpp_object_psdyn_main
.PHONY : popcpp_object_psdyn_main

# fast build rule for target.
popcpp_object_psdyn_main/fast:
	$(MAKE) -f lib/pseudodynamic/CMakeFiles/popcpp_object_psdyn_main.dir/build.make lib/pseudodynamic/CMakeFiles/popcpp_object_psdyn_main.dir/build
.PHONY : popcpp_object_psdyn_main/fast

#=============================================================================
# Target rules for targets named gen

# Build rule for target.
gen: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 gen
.PHONY : gen

# fast build rule for target.
gen/fast:
	$(MAKE) -f parser/CMakeFiles/gen.dir/build.make parser/CMakeFiles/gen.dir/build
.PHONY : gen/fast

#=============================================================================
# Target rules for targets named popcc

# Build rule for target.
popcc: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcc
.PHONY : popcc

# fast build rule for target.
popcc/fast:
	$(MAKE) -f parser/CMakeFiles/popcc.dir/build.make parser/CMakeFiles/popcc.dir/build
.PHONY : popcc/fast

#=============================================================================
# Target rules for targets named popcpp

# Build rule for target.
popcpp: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popcpp
.PHONY : popcpp

# fast build rule for target.
popcpp/fast:
	$(MAKE) -f parser/CMakeFiles/popcpp.dir/build.make parser/CMakeFiles/popcpp.dir/build
.PHONY : popcpp/fast

#=============================================================================
# Target rules for targets named openmpi_check_support

# Build rule for target.
openmpi_check_support: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 openmpi_check_support
.PHONY : openmpi_check_support

# fast build rule for target.
openmpi_check_support/fast:
	$(MAKE) -f interconnector/CMakeFiles/openmpi_check_support.dir/build.make interconnector/CMakeFiles/openmpi_check_support.dir/build
.PHONY : openmpi_check_support/fast

#=============================================================================
# Target rules for targets named popc_mpi_interconnector

# Build rule for target.
popc_mpi_interconnector: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_mpi_interconnector
.PHONY : popc_mpi_interconnector

# fast build rule for target.
popc_mpi_interconnector/fast:
	$(MAKE) -f interconnector/CMakeFiles/popc_mpi_interconnector.dir/build.make interconnector/CMakeFiles/popc_mpi_interconnector.dir/build
.PHONY : popc_mpi_interconnector/fast

#=============================================================================
# Target rules for targets named popc_resource_manager_mpi

# Build rule for target.
popc_resource_manager_mpi: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_resource_manager_mpi
.PHONY : popc_resource_manager_mpi

# fast build rule for target.
popc_resource_manager_mpi/fast:
	$(MAKE) -f interconnector/CMakeFiles/popc_resource_manager_mpi.dir/build.make interconnector/CMakeFiles/popc_resource_manager_mpi.dir/build
.PHONY : popc_resource_manager_mpi/fast

#=============================================================================
# Target rules for targets named popc_objmain.xmp.o

# Build rule for target.
popc_objmain.xmp.o: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_objmain.xmp.o
.PHONY : popc_objmain.xmp.o

# fast build rule for target.
popc_objmain.xmp.o/fast:
	$(MAKE) -f xmp/CMakeFiles/popc_objmain.xmp.o.dir/build.make xmp/CMakeFiles/popc_objmain.xmp.o.dir/build
.PHONY : popc_objmain.xmp.o/fast

#=============================================================================
# Target rules for targets named popc_advanced

# Build rule for target.
popc_advanced: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 popc_advanced
.PHONY : popc_advanced

# fast build rule for target.
popc_advanced/fast:
	$(MAKE) -f xmp/src/CMakeFiles/popc_advanced.dir/build.make xmp/src/CMakeFiles/popc_advanced.dir/build
.PHONY : popc_advanced/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... distclean"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... popc_common"
	@echo "... popcpp_master_main"
	@echo "... popcpp_object_main"
	@echo "... popc_common_psdyn"
	@echo "... popcpp_master_psdyn_main"
	@echo "... popcpp_object_psdyn_main"
	@echo "... gen"
	@echo "... popcc"
	@echo "... popcpp"
	@echo "... openmpi_check_support"
	@echo "... popc_mpi_interconnector"
	@echo "... popc_resource_manager_mpi"
	@echo "... popc_objmain.xmp.o"
	@echo "... popc_advanced"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

