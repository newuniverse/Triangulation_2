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
include CMakeFiles/voronoi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/voronoi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/voronoi.dir/flags.make

CMakeFiles/voronoi.dir/voronoi.cpp.o: CMakeFiles/voronoi.dir/flags.make
CMakeFiles/voronoi.dir/voronoi.cpp.o: ../voronoi.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/NewUniverse/Documents/Triangulation_2/build0/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/voronoi.dir/voronoi.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/voronoi.dir/voronoi.cpp.o -c /Users/NewUniverse/Documents/Triangulation_2/voronoi.cpp

CMakeFiles/voronoi.dir/voronoi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/voronoi.dir/voronoi.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/NewUniverse/Documents/Triangulation_2/voronoi.cpp > CMakeFiles/voronoi.dir/voronoi.cpp.i

CMakeFiles/voronoi.dir/voronoi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/voronoi.dir/voronoi.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/NewUniverse/Documents/Triangulation_2/voronoi.cpp -o CMakeFiles/voronoi.dir/voronoi.cpp.s

CMakeFiles/voronoi.dir/voronoi.cpp.o.requires:
.PHONY : CMakeFiles/voronoi.dir/voronoi.cpp.o.requires

CMakeFiles/voronoi.dir/voronoi.cpp.o.provides: CMakeFiles/voronoi.dir/voronoi.cpp.o.requires
	$(MAKE) -f CMakeFiles/voronoi.dir/build.make CMakeFiles/voronoi.dir/voronoi.cpp.o.provides.build
.PHONY : CMakeFiles/voronoi.dir/voronoi.cpp.o.provides

CMakeFiles/voronoi.dir/voronoi.cpp.o.provides.build: CMakeFiles/voronoi.dir/voronoi.cpp.o

# Object files for target voronoi
voronoi_OBJECTS = \
"CMakeFiles/voronoi.dir/voronoi.cpp.o"

# External object files for target voronoi
voronoi_EXTERNAL_OBJECTS =

voronoi: CMakeFiles/voronoi.dir/voronoi.cpp.o
voronoi: CMakeFiles/voronoi.dir/build.make
voronoi: /usr/local/lib/libCGAL_Core.dylib
voronoi: /usr/local/lib/libCGAL.dylib
voronoi: /opt/local/lib/libgmpxx.dylib
voronoi: /opt/local/lib/libmpfr.dylib
voronoi: /opt/local/lib/libgmp.dylib
voronoi: /opt/local/lib/libboost_thread-mt.dylib
voronoi: /usr/local/lib/libCGAL_Core.dylib
voronoi: /usr/local/lib/libCGAL.dylib
voronoi: /opt/local/lib/libgmpxx.dylib
voronoi: /opt/local/lib/libmpfr.dylib
voronoi: /opt/local/lib/libgmp.dylib
voronoi: /opt/local/lib/libboost_thread-mt.dylib
voronoi: CMakeFiles/voronoi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable voronoi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/voronoi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/voronoi.dir/build: voronoi
.PHONY : CMakeFiles/voronoi.dir/build

CMakeFiles/voronoi.dir/requires: CMakeFiles/voronoi.dir/voronoi.cpp.o.requires
.PHONY : CMakeFiles/voronoi.dir/requires

CMakeFiles/voronoi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/voronoi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/voronoi.dir/clean

CMakeFiles/voronoi.dir/depend:
	cd /Users/NewUniverse/Documents/Triangulation_2/build0 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/NewUniverse/Documents/Triangulation_2 /Users/NewUniverse/Documents/Triangulation_2 /Users/NewUniverse/Documents/Triangulation_2/build0 /Users/NewUniverse/Documents/Triangulation_2/build0 /Users/NewUniverse/Documents/Triangulation_2/build0/CMakeFiles/voronoi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/voronoi.dir/depend

