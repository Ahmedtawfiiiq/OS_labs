# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir

# Include any dependencies generated for this target.
include CMakeFiles/project3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/project3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project3.dir/flags.make

CMakeFiles/project3.dir/main.cc.o: CMakeFiles/project3.dir/flags.make
CMakeFiles/project3.dir/main.cc.o: ../main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/project3.dir/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/project3.dir/main.cc.o -c /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/main.cc

CMakeFiles/project3.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project3.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/main.cc > CMakeFiles/project3.dir/main.cc.i

CMakeFiles/project3.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project3.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/main.cc -o CMakeFiles/project3.dir/main.cc.s

CMakeFiles/project3.dir/utilities.cc.o: CMakeFiles/project3.dir/flags.make
CMakeFiles/project3.dir/utilities.cc.o: ../utilities.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/project3.dir/utilities.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/project3.dir/utilities.cc.o -c /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/utilities.cc

CMakeFiles/project3.dir/utilities.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project3.dir/utilities.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/utilities.cc > CMakeFiles/project3.dir/utilities.cc.i

CMakeFiles/project3.dir/utilities.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project3.dir/utilities.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/utilities.cc -o CMakeFiles/project3.dir/utilities.cc.s

# Object files for target project3
project3_OBJECTS = \
"CMakeFiles/project3.dir/main.cc.o" \
"CMakeFiles/project3.dir/utilities.cc.o"

# External object files for target project3
project3_EXTERNAL_OBJECTS =

lab3: CMakeFiles/project3.dir/main.cc.o
lab3: CMakeFiles/project3.dir/utilities.cc.o
lab3: CMakeFiles/project3.dir/build.make
lab3: CMakeFiles/project3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lab3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/project3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project3.dir/build: lab3

.PHONY : CMakeFiles/project3.dir/build

CMakeFiles/project3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/project3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/project3.dir/clean

CMakeFiles/project3.dir/depend:
	cd /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3 /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3 /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir /home/ahmed/Documents/os-labs/OS_labs/6842-lab3/test/project3/build-dir/CMakeFiles/project3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project3.dir/depend

