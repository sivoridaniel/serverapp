# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/agustin/git/serverapp/doxygen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/agustin/git/serverapp/doxygen

# Include any dependencies generated for this target.
include src/CMakeFiles/doxygen.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/doxygen.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/doxygen.dir/flags.make

src/CMakeFiles/doxygen.dir/main.cpp.o: src/CMakeFiles/doxygen.dir/flags.make
src/CMakeFiles/doxygen.dir/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/agustin/git/serverapp/doxygen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/doxygen.dir/main.cpp.o"
	cd /home/agustin/git/serverapp/doxygen/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doxygen.dir/main.cpp.o -c /home/agustin/git/serverapp/doxygen/src/main.cpp

src/CMakeFiles/doxygen.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doxygen.dir/main.cpp.i"
	cd /home/agustin/git/serverapp/doxygen/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/agustin/git/serverapp/doxygen/src/main.cpp > CMakeFiles/doxygen.dir/main.cpp.i

src/CMakeFiles/doxygen.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doxygen.dir/main.cpp.s"
	cd /home/agustin/git/serverapp/doxygen/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/agustin/git/serverapp/doxygen/src/main.cpp -o CMakeFiles/doxygen.dir/main.cpp.s

src/CMakeFiles/doxygen.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/doxygen.dir/main.cpp.o.requires

src/CMakeFiles/doxygen.dir/main.cpp.o.provides: src/CMakeFiles/doxygen.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/doxygen.dir/build.make src/CMakeFiles/doxygen.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/doxygen.dir/main.cpp.o.provides

src/CMakeFiles/doxygen.dir/main.cpp.o.provides.build: src/CMakeFiles/doxygen.dir/main.cpp.o


# Object files for target doxygen
doxygen_OBJECTS = \
"CMakeFiles/doxygen.dir/main.cpp.o"

# External object files for target doxygen
doxygen_EXTERNAL_OBJECTS =

bin/doxygen: src/CMakeFiles/doxygen.dir/main.cpp.o
bin/doxygen: src/CMakeFiles/doxygen.dir/build.make
bin/doxygen: lib/lib_doxygen.a
bin/doxygen: lib/libdoxycfg.a
bin/doxygen: lib/libqtools.a
bin/doxygen: lib/libmd5.a
bin/doxygen: lib/libvhdlparser.a
bin/doxygen: src/CMakeFiles/doxygen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/agustin/git/serverapp/doxygen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/doxygen"
	cd /home/agustin/git/serverapp/doxygen/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/doxygen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/doxygen.dir/build: bin/doxygen

.PHONY : src/CMakeFiles/doxygen.dir/build

# Object files for target doxygen
doxygen_OBJECTS = \
"CMakeFiles/doxygen.dir/main.cpp.o"

# External object files for target doxygen
doxygen_EXTERNAL_OBJECTS =

src/CMakeFiles/CMakeRelink.dir/doxygen: src/CMakeFiles/doxygen.dir/main.cpp.o
src/CMakeFiles/CMakeRelink.dir/doxygen: src/CMakeFiles/doxygen.dir/build.make
src/CMakeFiles/CMakeRelink.dir/doxygen: lib/lib_doxygen.a
src/CMakeFiles/CMakeRelink.dir/doxygen: lib/libdoxycfg.a
src/CMakeFiles/CMakeRelink.dir/doxygen: lib/libqtools.a
src/CMakeFiles/CMakeRelink.dir/doxygen: lib/libmd5.a
src/CMakeFiles/CMakeRelink.dir/doxygen: lib/libvhdlparser.a
src/CMakeFiles/CMakeRelink.dir/doxygen: src/CMakeFiles/doxygen.dir/relink.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/agustin/git/serverapp/doxygen/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable CMakeFiles/CMakeRelink.dir/doxygen"
	cd /home/agustin/git/serverapp/doxygen/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/doxygen.dir/relink.txt --verbose=$(VERBOSE)

# Rule to relink during preinstall.
src/CMakeFiles/doxygen.dir/preinstall: src/CMakeFiles/CMakeRelink.dir/doxygen

.PHONY : src/CMakeFiles/doxygen.dir/preinstall

src/CMakeFiles/doxygen.dir/requires: src/CMakeFiles/doxygen.dir/main.cpp.o.requires

.PHONY : src/CMakeFiles/doxygen.dir/requires

src/CMakeFiles/doxygen.dir/clean:
	cd /home/agustin/git/serverapp/doxygen/src && $(CMAKE_COMMAND) -P CMakeFiles/doxygen.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/doxygen.dir/clean

src/CMakeFiles/doxygen.dir/depend:
	cd /home/agustin/git/serverapp/doxygen && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/agustin/git/serverapp/doxygen /home/agustin/git/serverapp/doxygen/src /home/agustin/git/serverapp/doxygen /home/agustin/git/serverapp/doxygen/src /home/agustin/git/serverapp/doxygen/src/CMakeFiles/doxygen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/doxygen.dir/depend

