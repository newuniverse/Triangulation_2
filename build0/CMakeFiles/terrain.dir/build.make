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
CMAKE_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-9.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/NewUniverse/Documents/Triangulation_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/NewUniverse/Documents/Triangulation_2/build0

# Include any dependencies generated for this target.
include CMakeFiles/terrain.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/terrain.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/terrain.dir/flags.make

CMakeFiles/terrain.dir/terrain.cpp.o: CMakeFiles/terrain.dir/flags.make
CMakeFiles/terrain.dir/terrain.cpp.o: ../terrain.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/NewUniverse/Documents/Triangulation_2/build0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/terrain.dir/terrain.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/terrain.dir/terrain.cpp.o -c /Users/NewUniverse/Documents/Triangulation_2/terrain.cpp

CMakeFiles/terrain.dir/terrain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/terrain.dir/terrain.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/NewUniverse/Documents/Triangulation_2/terrain.cpp > CMakeFiles/terrain.dir/terrain.cpp.i

CMakeFiles/terrain.dir/terrain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/terrain.dir/terrain.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/NewUniverse/Documents/Triangulation_2/terrain.cpp -o CMakeFiles/terrain.dir/terrain.cpp.s

CMakeFiles/terrain.dir/terrain.cpp.o.requires:
.PHONY : CMakeFiles/terrain.dir/terrain.cpp.o.requires

CMakeFiles/terrain.dir/terrain.cpp.o.provides: CMakeFiles/terrain.dir/terrain.cpp.o.requires
	$(MAKE) -f CMakeFiles/terrain.dir/build.make CMakeFiles/terrain.dir/terrain.cpp.o.provides.build
.PHONY : CMakeFiles/terrain.dir/terrain.cpp.o.provides

CMakeFiles/terrain.dir/terrain.cpp.o.provides.build: CMakeFiles/terrain.dir/terrain.cpp.o

# Object files for target terrain
terrain_OBJECTS = \
"CMakeFiles/terrain.dir/terrain.cpp.o"

# External object files for target terrain
terrain_EXTERNAL_OBJECTS =

terrain: CMakeFiles/terrain.dir/terrain.cpp.o
terrain: CMakeFiles/terrain.dir/build.make
terrain: /usr/local/lib/libCGAL_Core.dylib
terrain: /usr/local/lib/libCGAL.dylib
terrain: /opt/local/lib/libgmpxx.dylib
terrain: /opt/local/lib/libmpfr.dylib
terrain: /opt/local/lib/libgmp.dylib
terrain: /opt/local/lib/libboost_thread-mt.dylib
terrain: /usr/local/lib/libCGAL_Core.dylib
terrain: /usr/local/lib/libCGAL.dylib
terrain: /opt/local/lib/libgmpxx.dylib
terrain: /opt/local/lib/libmpfr.dylib
terrain: /opt/local/lib/libgmp.dylib
terrain: /opt/local/lib/libboost_thread-mt.dylib
terrain: CMakeFiles/terrain.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable terrain"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/terrain.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/terrain.dir/build: terrain
.PHONY : CMakeFiles/terrain.dir/build

CMakeFiles/terrain.dir/requires: CMakeFiles/terrain.dir/terrain.cpp.o.requires
.PHONY : CMakeFiles/terrain.dir/requires

CMakeFiles/terrain.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/terrain.dir/cmake_clean.cmake
.PHONY : CMakeFiles/terrain.dir/clean

CMakeFiles/terrain.dir/depend:
	cd /Users/NewUniverse/Documents/Triangulation_2/build0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/NewUniverse/Documents/Triangulation_2 /Users/NewUniverse/Documents/Triangulation_2 /Users/NewUniverse/Documents/Triangulation_2/build0 /Users/NewUniverse/Documents/Triangulation_2/build0 /Users/NewUniverse/Documents/Triangulation_2/build0/CMakeFiles/terrain.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/terrain.dir/depend

