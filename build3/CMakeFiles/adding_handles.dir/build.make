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
include CMakeFiles/adding_handles.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/adding_handles.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/adding_handles.dir/flags.make

CMakeFiles/adding_handles.dir/adding_handles.cpp.o: CMakeFiles/adding_handles.dir/flags.make
CMakeFiles/adding_handles.dir/adding_handles.cpp.o: ../adding_handles.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/adding_handles.dir/adding_handles.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/adding_handles.dir/adding_handles.cpp.o -c /Users/NewUniverse/Documents/porousMetal/Triangulation_2/adding_handles.cpp

CMakeFiles/adding_handles.dir/adding_handles.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/adding_handles.dir/adding_handles.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/NewUniverse/Documents/porousMetal/Triangulation_2/adding_handles.cpp > CMakeFiles/adding_handles.dir/adding_handles.cpp.i

CMakeFiles/adding_handles.dir/adding_handles.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/adding_handles.dir/adding_handles.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/NewUniverse/Documents/porousMetal/Triangulation_2/adding_handles.cpp -o CMakeFiles/adding_handles.dir/adding_handles.cpp.s

CMakeFiles/adding_handles.dir/adding_handles.cpp.o.requires:
.PHONY : CMakeFiles/adding_handles.dir/adding_handles.cpp.o.requires

CMakeFiles/adding_handles.dir/adding_handles.cpp.o.provides: CMakeFiles/adding_handles.dir/adding_handles.cpp.o.requires
	$(MAKE) -f CMakeFiles/adding_handles.dir/build.make CMakeFiles/adding_handles.dir/adding_handles.cpp.o.provides.build
.PHONY : CMakeFiles/adding_handles.dir/adding_handles.cpp.o.provides

CMakeFiles/adding_handles.dir/adding_handles.cpp.o.provides.build: CMakeFiles/adding_handles.dir/adding_handles.cpp.o

# Object files for target adding_handles
adding_handles_OBJECTS = \
"CMakeFiles/adding_handles.dir/adding_handles.cpp.o"

# External object files for target adding_handles
adding_handles_EXTERNAL_OBJECTS =

adding_handles: CMakeFiles/adding_handles.dir/adding_handles.cpp.o
adding_handles: CMakeFiles/adding_handles.dir/build.make
adding_handles: /usr/local/lib/libCGAL_Core.dylib
adding_handles: /usr/local/lib/libCGAL.dylib
adding_handles: /opt/local/lib/libgmpxx.dylib
adding_handles: /opt/local/lib/libmpfr.dylib
adding_handles: /opt/local/lib/libgmp.dylib
adding_handles: /opt/local/lib/libboost_thread-mt.dylib
adding_handles: /usr/local/lib/libCGAL_Core.dylib
adding_handles: /usr/local/lib/libCGAL.dylib
adding_handles: /opt/local/lib/libgmpxx.dylib
adding_handles: /opt/local/lib/libmpfr.dylib
adding_handles: /opt/local/lib/libgmp.dylib
adding_handles: /opt/local/lib/libboost_thread-mt.dylib
adding_handles: CMakeFiles/adding_handles.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable adding_handles"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/adding_handles.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/adding_handles.dir/build: adding_handles
.PHONY : CMakeFiles/adding_handles.dir/build

CMakeFiles/adding_handles.dir/requires: CMakeFiles/adding_handles.dir/adding_handles.cpp.o.requires
.PHONY : CMakeFiles/adding_handles.dir/requires

CMakeFiles/adding_handles.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/adding_handles.dir/cmake_clean.cmake
.PHONY : CMakeFiles/adding_handles.dir/clean

CMakeFiles/adding_handles.dir/depend:
	cd /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/NewUniverse/Documents/porousMetal/Triangulation_2 /Users/NewUniverse/Documents/porousMetal/Triangulation_2 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3 /Users/NewUniverse/Documents/porousMetal/Triangulation_2/build3/CMakeFiles/adding_handles.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/adding_handles.dir/depend

