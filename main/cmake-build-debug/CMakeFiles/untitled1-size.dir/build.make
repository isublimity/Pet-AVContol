# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/igor/sites/Pet-AVContol/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/igor/sites/Pet-AVContol/main/cmake-build-debug

# Utility rule file for untitled1-size.

# Include the progress variables for this target.
include CMakeFiles/untitled1-size.dir/progress.make

CMakeFiles/untitled1-size: untitled1.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/igor/sites/Pet-AVContol/main/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Calculating untitled1 image size"
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -DFIRMWARE_IMAGE=/Users/igor/sites/Pet-AVContol/main/cmake-build-debug/untitled1.elf -DMCU=atmega2560 -DEEPROM_IMAGE=/Users/igor/sites/Pet-AVContol/main/cmake-build-debug/untitled1.eep -P /Users/igor/sites/Pet-AVContol/main/cmake-build-debug/CMakeFiles/FirmwareSize.cmake

untitled1-size: CMakeFiles/untitled1-size
untitled1-size: CMakeFiles/untitled1-size.dir/build.make

.PHONY : untitled1-size

# Rule to build all files generated by this target.
CMakeFiles/untitled1-size.dir/build: untitled1-size

.PHONY : CMakeFiles/untitled1-size.dir/build

CMakeFiles/untitled1-size.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/untitled1-size.dir/cmake_clean.cmake
.PHONY : CMakeFiles/untitled1-size.dir/clean

CMakeFiles/untitled1-size.dir/depend:
	cd /Users/igor/sites/Pet-AVContol/main/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/igor/sites/Pet-AVContol/main /Users/igor/sites/Pet-AVContol/main /Users/igor/sites/Pet-AVContol/main/cmake-build-debug /Users/igor/sites/Pet-AVContol/main/cmake-build-debug /Users/igor/sites/Pet-AVContol/main/cmake-build-debug/CMakeFiles/untitled1-size.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/untitled1-size.dir/depend

