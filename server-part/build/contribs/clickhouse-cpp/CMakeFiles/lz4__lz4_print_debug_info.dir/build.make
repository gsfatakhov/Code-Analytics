# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/projects/clickhouse-app

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/projects/clickhouse-app/build

# Utility rule file for lz4__lz4_print_debug_info.

# Include any custom commands dependencies for this target.
include contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/compiler_depend.make

# Include the progress variables for this target.
include contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/progress.make

contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info:
	cd /data/projects/clickhouse-app/build/contribs/clickhouse-cpp && /usr/bin/cmake -E echo lz4::lz4\ :\ /data/projects/clickhouse-app/build/contribs/clickhouse-cpp/contrib/lz4/lz4/liblz4.a

lz4__lz4_print_debug_info: contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info
lz4__lz4_print_debug_info: contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/build.make
.PHONY : lz4__lz4_print_debug_info

# Rule to build all files generated by this target.
contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/build: lz4__lz4_print_debug_info
.PHONY : contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/build

contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/clean:
	cd /data/projects/clickhouse-app/build/contribs/clickhouse-cpp && $(CMAKE_COMMAND) -P CMakeFiles/lz4__lz4_print_debug_info.dir/cmake_clean.cmake
.PHONY : contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/clean

contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/depend:
	cd /data/projects/clickhouse-app/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/projects/clickhouse-app /data/projects/clickhouse-app/contribs/clickhouse-cpp /data/projects/clickhouse-app/build /data/projects/clickhouse-app/build/contribs/clickhouse-cpp /data/projects/clickhouse-app/build/contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contribs/clickhouse-cpp/CMakeFiles/lz4__lz4_print_debug_info.dir/depend

