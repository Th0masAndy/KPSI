# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andy/MPC/KPSI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andy/MPC/KPSI

# Include any dependencies generated for this target.
include src/CMakeFiles/kpsi.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/kpsi.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/kpsi.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/kpsi.dir/flags.make

src/CMakeFiles/kpsi.dir/main.cpp.o: src/CMakeFiles/kpsi.dir/flags.make
src/CMakeFiles/kpsi.dir/main.cpp.o: src/main.cpp
src/CMakeFiles/kpsi.dir/main.cpp.o: src/CMakeFiles/kpsi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/andy/MPC/KPSI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/kpsi.dir/main.cpp.o"
	cd /home/andy/MPC/KPSI/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/kpsi.dir/main.cpp.o -MF CMakeFiles/kpsi.dir/main.cpp.o.d -o CMakeFiles/kpsi.dir/main.cpp.o -c /home/andy/MPC/KPSI/src/main.cpp

src/CMakeFiles/kpsi.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kpsi.dir/main.cpp.i"
	cd /home/andy/MPC/KPSI/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/andy/MPC/KPSI/src/main.cpp > CMakeFiles/kpsi.dir/main.cpp.i

src/CMakeFiles/kpsi.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kpsi.dir/main.cpp.s"
	cd /home/andy/MPC/KPSI/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/andy/MPC/KPSI/src/main.cpp -o CMakeFiles/kpsi.dir/main.cpp.s

# Object files for target kpsi
kpsi_OBJECTS = \
"CMakeFiles/kpsi.dir/main.cpp.o"

# External object files for target kpsi
kpsi_EXTERNAL_OBJECTS =

kpsi: src/CMakeFiles/kpsi.dir/main.cpp.o
kpsi: src/CMakeFiles/kpsi.dir/build.make
kpsi: /usr/local/lib/libseal-4.1.a
kpsi: src/CMakeFiles/kpsi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/andy/MPC/KPSI/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../kpsi"
	cd /home/andy/MPC/KPSI/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kpsi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/kpsi.dir/build: kpsi
.PHONY : src/CMakeFiles/kpsi.dir/build

src/CMakeFiles/kpsi.dir/clean:
	cd /home/andy/MPC/KPSI/src && $(CMAKE_COMMAND) -P CMakeFiles/kpsi.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/kpsi.dir/clean

src/CMakeFiles/kpsi.dir/depend:
	cd /home/andy/MPC/KPSI && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andy/MPC/KPSI /home/andy/MPC/KPSI/src /home/andy/MPC/KPSI /home/andy/MPC/KPSI/src /home/andy/MPC/KPSI/src/CMakeFiles/kpsi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/kpsi.dir/depend
