# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient

# Include any dependencies generated for this target.
include CMakeFiles/udpConsole.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/udpConsole.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/udpConsole.dir/flags.make

CMakeFiles/udpConsole.dir/udpconsole.c.o: CMakeFiles/udpConsole.dir/flags.make
CMakeFiles/udpConsole.dir/udpconsole.c.o: udpconsole.c
	$(CMAKE_COMMAND) -E cmake_progress_report /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/udpConsole.dir/udpconsole.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/udpConsole.dir/udpconsole.c.o   -c /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/udpconsole.c

CMakeFiles/udpConsole.dir/udpconsole.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/udpConsole.dir/udpconsole.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/udpconsole.c > CMakeFiles/udpConsole.dir/udpconsole.c.i

CMakeFiles/udpConsole.dir/udpconsole.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/udpConsole.dir/udpconsole.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/udpconsole.c -o CMakeFiles/udpConsole.dir/udpconsole.c.s

CMakeFiles/udpConsole.dir/udpconsole.c.o.requires:
.PHONY : CMakeFiles/udpConsole.dir/udpconsole.c.o.requires

CMakeFiles/udpConsole.dir/udpconsole.c.o.provides: CMakeFiles/udpConsole.dir/udpconsole.c.o.requires
	$(MAKE) -f CMakeFiles/udpConsole.dir/build.make CMakeFiles/udpConsole.dir/udpconsole.c.o.provides.build
.PHONY : CMakeFiles/udpConsole.dir/udpconsole.c.o.provides

CMakeFiles/udpConsole.dir/udpconsole.c.o.provides.build: CMakeFiles/udpConsole.dir/udpconsole.c.o
.PHONY : CMakeFiles/udpConsole.dir/udpconsole.c.o.provides.build

CMakeFiles/udpConsole.dir/vt100.c.o: CMakeFiles/udpConsole.dir/flags.make
CMakeFiles/udpConsole.dir/vt100.c.o: vt100.c
	$(CMAKE_COMMAND) -E cmake_progress_report /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/udpConsole.dir/vt100.c.o"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/udpConsole.dir/vt100.c.o   -c /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/vt100.c

CMakeFiles/udpConsole.dir/vt100.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/udpConsole.dir/vt100.c.i"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/vt100.c > CMakeFiles/udpConsole.dir/vt100.c.i

CMakeFiles/udpConsole.dir/vt100.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/udpConsole.dir/vt100.c.s"
	/usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/vt100.c -o CMakeFiles/udpConsole.dir/vt100.c.s

CMakeFiles/udpConsole.dir/vt100.c.o.requires:
.PHONY : CMakeFiles/udpConsole.dir/vt100.c.o.requires

CMakeFiles/udpConsole.dir/vt100.c.o.provides: CMakeFiles/udpConsole.dir/vt100.c.o.requires
	$(MAKE) -f CMakeFiles/udpConsole.dir/build.make CMakeFiles/udpConsole.dir/vt100.c.o.provides.build
.PHONY : CMakeFiles/udpConsole.dir/vt100.c.o.provides

CMakeFiles/udpConsole.dir/vt100.c.o.provides.build: CMakeFiles/udpConsole.dir/vt100.c.o
.PHONY : CMakeFiles/udpConsole.dir/vt100.c.o.provides.build

# Object files for target udpConsole
udpConsole_OBJECTS = \
"CMakeFiles/udpConsole.dir/udpconsole.c.o" \
"CMakeFiles/udpConsole.dir/vt100.c.o"

# External object files for target udpConsole
udpConsole_EXTERNAL_OBJECTS =

udpConsole: CMakeFiles/udpConsole.dir/udpconsole.c.o
udpConsole: CMakeFiles/udpConsole.dir/vt100.c.o
udpConsole: CMakeFiles/udpConsole.dir/build.make
udpConsole: CMakeFiles/udpConsole.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable udpConsole"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/udpConsole.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/udpConsole.dir/build: udpConsole
.PHONY : CMakeFiles/udpConsole.dir/build

CMakeFiles/udpConsole.dir/requires: CMakeFiles/udpConsole.dir/udpconsole.c.o.requires
CMakeFiles/udpConsole.dir/requires: CMakeFiles/udpConsole.dir/vt100.c.o.requires
.PHONY : CMakeFiles/udpConsole.dir/requires

CMakeFiles/udpConsole.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/udpConsole.dir/cmake_clean.cmake
.PHONY : CMakeFiles/udpConsole.dir/clean

CMakeFiles/udpConsole.dir/depend:
	cd /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient /media/truecrypt1/mgr/withCLI/svn/Tools/udpClient/CMakeFiles/udpConsole.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/udpConsole.dir/depend

