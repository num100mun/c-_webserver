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
include CMakeFiles/C__EasyTcpServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/C__EasyTcpServer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/C__EasyTcpServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/C__EasyTcpServer.dir/flags.make

CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o: CMakeFiles/C__EasyTcpServer.dir/flags.make
CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o: /home/webserver/EasyTcpServer.cpp
CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o: CMakeFiles/C__EasyTcpServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o -MF CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o.d -o CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o -c /home/webserver/EasyTcpServer.cpp

CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/webserver/EasyTcpServer.cpp > CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.i

CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/webserver/EasyTcpServer.cpp -o CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.s

# Object files for target C__EasyTcpServer
C__EasyTcpServer_OBJECTS = \
"CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o"

# External object files for target C__EasyTcpServer
C__EasyTcpServer_EXTERNAL_OBJECTS =

C__EasyTcpServer: CMakeFiles/C__EasyTcpServer.dir/EasyTcpServer.cpp.o
C__EasyTcpServer: CMakeFiles/C__EasyTcpServer.dir/build.make
C__EasyTcpServer: CMakeFiles/C__EasyTcpServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/webserver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable C__EasyTcpServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/C__EasyTcpServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/C__EasyTcpServer.dir/build: C__EasyTcpServer
.PHONY : CMakeFiles/C__EasyTcpServer.dir/build

CMakeFiles/C__EasyTcpServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/C__EasyTcpServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/C__EasyTcpServer.dir/clean

CMakeFiles/C__EasyTcpServer.dir/depend:
	cd /home/webserver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/webserver /home/webserver /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug /home/webserver/cmake-build-debug/CMakeFiles/C__EasyTcpServer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/C__EasyTcpServer.dir/depend

