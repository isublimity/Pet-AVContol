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

# Utility rule file for untitled1-burn.

# Include the progress variables for this target.
include CMakeFiles/untitled1-burn.dir/progress.make

CMakeFiles/untitled1-burn: untitled1.elf
	/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/avrdude -C/Applications/Arduino.app/Contents/Java/hardware/tools/avr/etc/avrdude.conf -cstk500v1 -P/dev/cu.wchusbserialfa1220 -patmega2560 -v -Uflash:w:/Users/igor/sites/Pet-AVContol/main/cmake-build-debug/untitled1.hex

untitled1-burn: CMakeFiles/untitled1-burn
untitled1-burn: CMakeFiles/untitled1-burn.dir/build.make

.PHONY : untitled1-burn

# Rule to build all files generated by this target.
CMakeFiles/untitled1-burn.dir/build: untitled1-burn

.PHONY : CMakeFiles/untitled1-burn.dir/build

CMakeFiles/untitled1-burn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/untitled1-burn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/untitled1-burn.dir/clean

CMakeFiles/untitled1-burn.dir/depend:
	cd /Users/igor/sites/Pet-AVContol/main/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/igor/sites/Pet-AVContol/main /Users/igor/sites/Pet-AVContol/main /Users/igor/sites/Pet-AVContol/main/cmake-build-debug /Users/igor/sites/Pet-AVContol/main/cmake-build-debug /Users/igor/sites/Pet-AVContol/main/cmake-build-debug/CMakeFiles/untitled1-burn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/untitled1-burn.dir/depend

