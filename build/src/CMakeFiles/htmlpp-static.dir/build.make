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
CMAKE_SOURCE_DIR = /home/jan.koester/projects/libhtmlpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jan.koester/projects/libhtmlpp/build

# Include any dependencies generated for this target.
include src/CMakeFiles/htmlpp-static.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/htmlpp-static.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/htmlpp-static.dir/flags.make

src/CMakeFiles/htmlpp-static.dir/hstring.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/hstring.cpp.o: ../src/hstring.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/htmlpp-static.dir/hstring.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/hstring.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/hstring.cpp

src/CMakeFiles/htmlpp-static.dir/hstring.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/hstring.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/hstring.cpp > CMakeFiles/htmlpp-static.dir/hstring.cpp.i

src/CMakeFiles/htmlpp-static.dir/hstring.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/hstring.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/hstring.cpp -o CMakeFiles/htmlpp-static.dir/hstring.cpp.s

src/CMakeFiles/htmlpp-static.dir/css.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/css.cpp.o: ../src/css.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/htmlpp-static.dir/css.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/css.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/css.cpp

src/CMakeFiles/htmlpp-static.dir/css.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/css.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/css.cpp > CMakeFiles/htmlpp-static.dir/css.cpp.i

src/CMakeFiles/htmlpp-static.dir/css.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/css.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/css.cpp -o CMakeFiles/htmlpp-static.dir/css.cpp.s

src/CMakeFiles/htmlpp-static.dir/html.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/html.cpp.o: ../src/html.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/htmlpp-static.dir/html.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/html.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/html.cpp

src/CMakeFiles/htmlpp-static.dir/html.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/html.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/html.cpp > CMakeFiles/htmlpp-static.dir/html.cpp.i

src/CMakeFiles/htmlpp-static.dir/html.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/html.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/html.cpp -o CMakeFiles/htmlpp-static.dir/html.cpp.s

src/CMakeFiles/htmlpp-static.dir/request.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/request.cpp.o: ../src/request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/htmlpp-static.dir/request.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/request.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/request.cpp

src/CMakeFiles/htmlpp-static.dir/request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/request.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/request.cpp > CMakeFiles/htmlpp-static.dir/request.cpp.i

src/CMakeFiles/htmlpp-static.dir/request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/request.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/request.cpp -o CMakeFiles/htmlpp-static.dir/request.cpp.s

src/CMakeFiles/htmlpp-static.dir/system.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/system.cpp.o: ../src/system.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/htmlpp-static.dir/system.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/system.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/system.cpp

src/CMakeFiles/htmlpp-static.dir/system.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/system.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/system.cpp > CMakeFiles/htmlpp-static.dir/system.cpp.i

src/CMakeFiles/htmlpp-static.dir/system.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/system.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/system.cpp -o CMakeFiles/htmlpp-static.dir/system.cpp.s

src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o: ../src/request/ajax.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/request/ajax.cpp

src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/request/ajax.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/request/ajax.cpp > CMakeFiles/htmlpp-static.dir/request/ajax.cpp.i

src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/request/ajax.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/request/ajax.cpp -o CMakeFiles/htmlpp-static.dir/request/ajax.cpp.s

src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o: ../src/request/httpform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/request/httpform.cpp

src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/request/httpform.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/request/httpform.cpp > CMakeFiles/htmlpp-static.dir/request/httpform.cpp.i

src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/request/httpform.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/request/httpform.cpp -o CMakeFiles/htmlpp-static.dir/request/httpform.cpp.s

src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o: src/CMakeFiles/htmlpp-static.dir/flags.make
src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o: ../src/request/websocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o -c /home/jan.koester/projects/libhtmlpp/src/request/websocket.cpp

src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/htmlpp-static.dir/request/websocket.cpp.i"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jan.koester/projects/libhtmlpp/src/request/websocket.cpp > CMakeFiles/htmlpp-static.dir/request/websocket.cpp.i

src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/htmlpp-static.dir/request/websocket.cpp.s"
	cd /home/jan.koester/projects/libhtmlpp/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jan.koester/projects/libhtmlpp/src/request/websocket.cpp -o CMakeFiles/htmlpp-static.dir/request/websocket.cpp.s

# Object files for target htmlpp-static
htmlpp__static_OBJECTS = \
"CMakeFiles/htmlpp-static.dir/hstring.cpp.o" \
"CMakeFiles/htmlpp-static.dir/css.cpp.o" \
"CMakeFiles/htmlpp-static.dir/html.cpp.o" \
"CMakeFiles/htmlpp-static.dir/request.cpp.o" \
"CMakeFiles/htmlpp-static.dir/system.cpp.o" \
"CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o" \
"CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o" \
"CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o"

# External object files for target htmlpp-static
htmlpp__static_EXTERNAL_OBJECTS =

src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/hstring.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/css.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/html.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/request.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/system.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/request/ajax.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/request/httpform.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/request/websocket.cpp.o
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/build.make
src/libhtmlpp-static.a: src/CMakeFiles/htmlpp-static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jan.koester/projects/libhtmlpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library libhtmlpp-static.a"
	cd /home/jan.koester/projects/libhtmlpp/build/src && $(CMAKE_COMMAND) -P CMakeFiles/htmlpp-static.dir/cmake_clean_target.cmake
	cd /home/jan.koester/projects/libhtmlpp/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/htmlpp-static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/htmlpp-static.dir/build: src/libhtmlpp-static.a

.PHONY : src/CMakeFiles/htmlpp-static.dir/build

src/CMakeFiles/htmlpp-static.dir/clean:
	cd /home/jan.koester/projects/libhtmlpp/build/src && $(CMAKE_COMMAND) -P CMakeFiles/htmlpp-static.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/htmlpp-static.dir/clean

src/CMakeFiles/htmlpp-static.dir/depend:
	cd /home/jan.koester/projects/libhtmlpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jan.koester/projects/libhtmlpp /home/jan.koester/projects/libhtmlpp/src /home/jan.koester/projects/libhtmlpp/build /home/jan.koester/projects/libhtmlpp/build/src /home/jan.koester/projects/libhtmlpp/build/src/CMakeFiles/htmlpp-static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/htmlpp-static.dir/depend

