# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /snap/cmake/1366/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1366/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/webserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/webserver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/C__main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/C__main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/C__main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/C__main.dir/flags.make

CMakeFiles/C__main.dir/main.cpp.o: CMakeFiles/C__main.dir/flags.make
CMakeFiles/C__main.dir/main.cpp.o: /home/webserver/main.cpp
CMakeFiles/C__main.dir/main.cpp.o: CMakeFiles/C__main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/C__main.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/C__main.dir/main.cpp.o -MF CMakeFiles/C__main.dir/main.cpp.o.d -o CMakeFiles/C__main.dir/main.cpp.o -c /home/webserver/main.cpp

CMakeFiles/C__main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/C__main.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/webserver/main.cpp > CMakeFiles/C__main.dir/main.cpp.i

CMakeFiles/C__main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/C__main.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/webserver/main.cpp -o CMakeFiles/C__main.dir/main.cpp.s

# Object files for target C__main
C__main_OBJECTS = \
"CMakeFiles/C__main.dir/main.cpp.o"

# External object files for target C__main
C__main_EXTERNAL_OBJECTS =

C__main: CMakeFiles/C__main.dir/main.cpp.o
C__main: CMakeFiles/C__main.dir/build.make
C__main: CMakeFiles/C__main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable C__main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/C__main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/C__main.dir/build: C__main
.PHONY : CMakeFiles/C__main.dir/build

CMakeFiles/C__main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/C__main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/C__main.dir/clean

CMakeFiles/C__main.dir/depend:
	cd /home/webserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/webserver /home/webserver /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug/CMakeFiles/C__main.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/C__main.dir/depend

