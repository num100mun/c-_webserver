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
include CMakeFiles/C__EasyTcpClient.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/C__EasyTcpClient.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/C__EasyTcpClient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/C__EasyTcpClient.dir/flags.make

CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o: CMakeFiles/C__EasyTcpClient.dir/flags.make
CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o: /home/webserver/EasyTcpClient.cpp
CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o: CMakeFiles/C__EasyTcpClient.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o -MF CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o.d -o CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o -c /home/webserver/EasyTcpClient.cpp

CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/webserver/EasyTcpClient.cpp > CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.i

CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/webserver/EasyTcpClient.cpp -o CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.s

# Object files for target C__EasyTcpClient
C__EasyTcpClient_OBJECTS = \
"CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o"

# External object files for target C__EasyTcpClient
C__EasyTcpClient_EXTERNAL_OBJECTS =

C__EasyTcpClient: CMakeFiles/C__EasyTcpClient.dir/EasyTcpClient.cpp.o
C__EasyTcpClient: CMakeFiles/C__EasyTcpClient.dir/build.make
C__EasyTcpClient: CMakeFiles/C__EasyTcpClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable C__EasyTcpClient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/C__EasyTcpClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/C__EasyTcpClient.dir/build: C__EasyTcpClient
.PHONY : CMakeFiles/C__EasyTcpClient.dir/build

CMakeFiles/C__EasyTcpClient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/C__EasyTcpClient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/C__EasyTcpClient.dir/clean

CMakeFiles/C__EasyTcpClient.dir/depend:
	cd /home/webserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/webserver /home/webserver /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug/CMakeFiles/C__EasyTcpClient.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/C__EasyTcpClient.dir/depend

