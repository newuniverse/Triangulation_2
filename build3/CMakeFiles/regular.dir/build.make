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
CMAKE_SOURCE_DIR = /Users/NewUniverse/Documents/porousMetal/Triangulation_2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3

# Include any dependencies generated for this target.
include CMakeFiles/regular.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/regular.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/regular.dir/flags.make

CMakeFiles/regular.dir/regular.cpp.o: CMakeFiles/regular.dir/flags.make
CMakeFiles/regular.dir/regular.cpp.o: ../regular.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/regular.dir/regular.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/regular.dir/regular.cpp.o -c /Users/NewUniverse/Documents/porousMetal/Triangulation_2/regular.cpp

CMakeFiles/regular.dir/regular.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/regular.dir/regular.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/NewUniverse/Documents/porousMetal/Triangulation_2/regular.cpp > CMakeFiles/regular.dir/regular.cpp.i

CMakeFiles/regular.dir/regular.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/regular.dir/regular.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/NewUniverse/Documents/porousMetal/Triangulation_2/regular.cpp -o CMakeFiles/regular.dir/regular.cpp.s

CMakeFiles/regular.dir/regular.cpp.o.requires:
.PHONY : CMakeFiles/regular.dir/regular.cpp.o.requires

CMakeFiles/regular.dir/regular.cpp.o.provides: CMakeFiles/regular.dir/regular.cpp.o.requires
	$(MAKE) -f CMakeFiles/regular.dir/build.make CMakeFiles/regular.dir/regular.cpp.o.provides.build
.PHONY : CMakeFiles/regular.dir/regular.cpp.o.provides

CMakeFiles/regular.dir/regular.cpp.o.provides.build: CMakeFiles/regular.dir/regular.cpp.o

# Object files for target regular
regular_OBJECTS = \
"CMakeFiles/regular.dir/regular.cpp.o"

# External object files for target regular
regular_EXTERNAL_OBJECTS =

regular: CMakeFiles/regular.dir/regular.cpp.o
regular: CMakeFiles/regular.dir/build.make
regular: /usr/local/lib/libCGAL_Core.dylib
regular: /usr/local/lib/libCGAL.dylib
regular: /opt/local/lib/libgmpxx.dylib
regular: /opt/local/lib/libmpfr.dylib
regular: /opt/local/lib/libgmp.dylib
regular: /opt/local/lib/libboost_thread-mt.dylib
regular: /usr/local/lib/libCGAL_Core.dylib
regular: /usr/local/lib/libCGAL.dylib
regular: /opt/local/lib/libgmpxx.dylib
regular: /opt/local/lib/libmpfr.dylib
regular: /opt/local/lib/libgmp.dylib
regular: /opt/local/lib/libboost_thread-mt.dylib
regular: CMakeFiles/regular.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable regular"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/regular.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/regular.dir/build: regular
.PHONY : CMakeFiles/regular.dir/build

CMakeFiles/regular.dir/requires: CMakeFiles/regular.dir/regular.cpp.o.requires
.PHONY : CMakeFiles/regular.dir/requires

CMakeFiles/regular.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/regular.dir/cmake_clean.cmake
.PHONY : CMakeFiles/regular.dir/clean

CMakeFiles/regular.dir/depend:
	cd /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/NewUniverse/Documents/porousMetal/Triangulation_2 /Users/NewUniverse/Documents/porousMetal/Triangulation_2 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3/CMakeFiles/regular.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/regular.dir/depend

