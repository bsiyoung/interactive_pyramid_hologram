# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/park/Workspace/interactive_pyramid_hologram/3d_renderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build

# Include any dependencies generated for this target.
include external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/depend.make

# Include the progress variables for this target.
include external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/flags.make

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.o: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.o: ../external/glfw-3.1.2/tests/gamma.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.o"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gamma.dir/gamma.c.o -c /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/tests/gamma.c

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gamma.dir/gamma.c.i"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/tests/gamma.c > CMakeFiles/gamma.dir/gamma.c.i

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gamma.dir/gamma.c.s"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/tests/gamma.c -o CMakeFiles/gamma.dir/gamma.c.s

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/flags.make
external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o: ../external/glfw-3.1.2/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gamma.dir/__/deps/getopt.c.o -c /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/deps/getopt.c

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gamma.dir/__/deps/getopt.c.i"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/deps/getopt.c > CMakeFiles/gamma.dir/__/deps/getopt.c.i

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gamma.dir/__/deps/getopt.c.s"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/deps/getopt.c -o CMakeFiles/gamma.dir/__/deps/getopt.c.s

# Object files for target gamma
gamma_OBJECTS = \
"CMakeFiles/gamma.dir/gamma.c.o" \
"CMakeFiles/gamma.dir/__/deps/getopt.c.o"

# External object files for target gamma
gamma_EXTERNAL_OBJECTS =

external/glfw-3.1.2/tests/gamma: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/gamma.c.o
external/glfw-3.1.2/tests/gamma: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o
external/glfw-3.1.2/tests/gamma: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/build.make
external/glfw-3.1.2/tests/gamma: external/glfw-3.1.2/src/libglfw3.a
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/librt.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libm.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libX11.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libXrandr.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libXinerama.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libXi.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libXcursor.so
external/glfw-3.1.2/tests/gamma: /usr/lib/arm-linux-gnueabihf/libGL.so
external/glfw-3.1.2/tests/gamma: external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable gamma"
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gamma.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/build: external/glfw-3.1.2/tests/gamma

.PHONY : external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/build

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/clean:
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/gamma.dir/cmake_clean.cmake
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/clean

external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/depend:
	cd /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/park/Workspace/interactive_pyramid_hologram/3d_renderer /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/external/glfw-3.1.2/tests /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests /home/park/Workspace/interactive_pyramid_hologram/3d_renderer/build/external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.1.2/tests/CMakeFiles/gamma.dir/depend

