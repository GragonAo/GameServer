# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/bin/cmake3

# The command to remove a file.
RM = /usr/bin/cmake3 -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/workspace/GameServer/10_01_game/src/tools/robots

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/GameServer/10_01_game/src/tools/robots

# Include any dependencies generated for this target.
include CMakeFiles/serverd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/serverd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/serverd.dir/flags.make

CMakeFiles/serverd.dir/global_robots.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/global_robots.cpp.o: global_robots.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/serverd.dir/global_robots.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/global_robots.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/global_robots.cpp

CMakeFiles/serverd.dir/global_robots.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/global_robots.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/global_robots.cpp > CMakeFiles/serverd.dir/global_robots.cpp.i

CMakeFiles/serverd.dir/global_robots.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/global_robots.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/global_robots.cpp -o CMakeFiles/serverd.dir/global_robots.cpp.s

CMakeFiles/serverd.dir/main.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/serverd.dir/main.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/main.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/main.cpp

CMakeFiles/serverd.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/main.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/main.cpp > CMakeFiles/serverd.dir/main.cpp.i

CMakeFiles/serverd.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/main.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/main.cpp -o CMakeFiles/serverd.dir/main.cpp.s

CMakeFiles/serverd.dir/robot.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot.cpp.o: robot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/serverd.dir/robot.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot.cpp

CMakeFiles/serverd.dir/robot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot.cpp > CMakeFiles/serverd.dir/robot.cpp.i

CMakeFiles/serverd.dir/robot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot.cpp -o CMakeFiles/serverd.dir/robot.cpp.s

CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o: robot_component_gametoken.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_gametoken.cpp

CMakeFiles/serverd.dir/robot_component_gametoken.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_component_gametoken.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_gametoken.cpp > CMakeFiles/serverd.dir/robot_component_gametoken.cpp.i

CMakeFiles/serverd.dir/robot_component_gametoken.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_component_gametoken.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_gametoken.cpp -o CMakeFiles/serverd.dir/robot_component_gametoken.cpp.s

CMakeFiles/serverd.dir/robot_component_login.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_component_login.cpp.o: robot_component_login.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/serverd.dir/robot_component_login.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_component_login.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_login.cpp

CMakeFiles/serverd.dir/robot_component_login.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_component_login.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_login.cpp > CMakeFiles/serverd.dir/robot_component_login.cpp.i

CMakeFiles/serverd.dir/robot_component_login.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_component_login.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_component_login.cpp -o CMakeFiles/serverd.dir/robot_component_login.cpp.s

CMakeFiles/serverd.dir/robot_console_http.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_console_http.cpp.o: robot_console_http.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/serverd.dir/robot_console_http.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_console_http.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_http.cpp

CMakeFiles/serverd.dir/robot_console_http.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_console_http.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_http.cpp > CMakeFiles/serverd.dir/robot_console_http.cpp.i

CMakeFiles/serverd.dir/robot_console_http.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_console_http.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_http.cpp -o CMakeFiles/serverd.dir/robot_console_http.cpp.s

CMakeFiles/serverd.dir/robot_console_login.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_console_login.cpp.o: robot_console_login.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/serverd.dir/robot_console_login.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_console_login.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_login.cpp

CMakeFiles/serverd.dir/robot_console_login.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_console_login.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_login.cpp > CMakeFiles/serverd.dir/robot_console_login.cpp.i

CMakeFiles/serverd.dir/robot_console_login.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_console_login.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_console_login.cpp -o CMakeFiles/serverd.dir/robot_console_login.cpp.s

CMakeFiles/serverd.dir/robot_mgr.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_mgr.cpp.o: robot_mgr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/serverd.dir/robot_mgr.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_mgr.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_mgr.cpp

CMakeFiles/serverd.dir/robot_mgr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_mgr.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_mgr.cpp > CMakeFiles/serverd.dir/robot_mgr.cpp.i

CMakeFiles/serverd.dir/robot_mgr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_mgr.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_mgr.cpp -o CMakeFiles/serverd.dir/robot_mgr.cpp.s

CMakeFiles/serverd.dir/robot_state.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_state.cpp.o: robot_state.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/serverd.dir/robot_state.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_state.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state.cpp

CMakeFiles/serverd.dir/robot_state.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_state.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state.cpp > CMakeFiles/serverd.dir/robot_state.cpp.i

CMakeFiles/serverd.dir/robot_state.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_state.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state.cpp -o CMakeFiles/serverd.dir/robot_state.cpp.s

CMakeFiles/serverd.dir/robot_state_game.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_state_game.cpp.o: robot_state_game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/serverd.dir/robot_state_game.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_state_game.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_game.cpp

CMakeFiles/serverd.dir/robot_state_game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_state_game.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_game.cpp > CMakeFiles/serverd.dir/robot_state_game.cpp.i

CMakeFiles/serverd.dir/robot_state_game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_state_game.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_game.cpp -o CMakeFiles/serverd.dir/robot_state_game.cpp.s

CMakeFiles/serverd.dir/robot_state_http.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_state_http.cpp.o: robot_state_http.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/serverd.dir/robot_state_http.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_state_http.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_http.cpp

CMakeFiles/serverd.dir/robot_state_http.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_state_http.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_http.cpp > CMakeFiles/serverd.dir/robot_state_http.cpp.i

CMakeFiles/serverd.dir/robot_state_http.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_state_http.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_http.cpp -o CMakeFiles/serverd.dir/robot_state_http.cpp.s

CMakeFiles/serverd.dir/robot_state_login.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/robot_state_login.cpp.o: robot_state_login.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/serverd.dir/robot_state_login.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/robot_state_login.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_login.cpp

CMakeFiles/serverd.dir/robot_state_login.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/robot_state_login.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_login.cpp > CMakeFiles/serverd.dir/robot_state_login.cpp.i

CMakeFiles/serverd.dir/robot_state_login.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/robot_state_login.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/robot_state_login.cpp -o CMakeFiles/serverd.dir/robot_state_login.cpp.s

CMakeFiles/serverd.dir/test_http_login.cpp.o: CMakeFiles/serverd.dir/flags.make
CMakeFiles/serverd.dir/test_http_login.cpp.o: test_http_login.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/serverd.dir/test_http_login.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serverd.dir/test_http_login.cpp.o -c /root/workspace/GameServer/10_01_game/src/tools/robots/test_http_login.cpp

CMakeFiles/serverd.dir/test_http_login.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serverd.dir/test_http_login.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/10_01_game/src/tools/robots/test_http_login.cpp > CMakeFiles/serverd.dir/test_http_login.cpp.i

CMakeFiles/serverd.dir/test_http_login.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serverd.dir/test_http_login.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/10_01_game/src/tools/robots/test_http_login.cpp -o CMakeFiles/serverd.dir/test_http_login.cpp.s

# Object files for target serverd
serverd_OBJECTS = \
"CMakeFiles/serverd.dir/global_robots.cpp.o" \
"CMakeFiles/serverd.dir/main.cpp.o" \
"CMakeFiles/serverd.dir/robot.cpp.o" \
"CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o" \
"CMakeFiles/serverd.dir/robot_component_login.cpp.o" \
"CMakeFiles/serverd.dir/robot_console_http.cpp.o" \
"CMakeFiles/serverd.dir/robot_console_login.cpp.o" \
"CMakeFiles/serverd.dir/robot_mgr.cpp.o" \
"CMakeFiles/serverd.dir/robot_state.cpp.o" \
"CMakeFiles/serverd.dir/robot_state_game.cpp.o" \
"CMakeFiles/serverd.dir/robot_state_http.cpp.o" \
"CMakeFiles/serverd.dir/robot_state_login.cpp.o" \
"CMakeFiles/serverd.dir/test_http_login.cpp.o"

# External object files for target serverd
serverd_EXTERNAL_OBJECTS =

/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/global_robots.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/main.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_component_gametoken.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_component_login.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_console_http.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_console_login.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_mgr.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_state.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_state_game.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_state_http.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/robot_state_login.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/test_http_login.cpp.o
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/build.make
/root/workspace/GameServer/10_01_game/libs/libserverd.a: CMakeFiles/serverd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX static library /root/workspace/GameServer/10_01_game/libs/libserverd.a"
	$(CMAKE_COMMAND) -P CMakeFiles/serverd.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serverd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/serverd.dir/build: /root/workspace/GameServer/10_01_game/libs/libserverd.a

.PHONY : CMakeFiles/serverd.dir/build

CMakeFiles/serverd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/serverd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/serverd.dir/clean

CMakeFiles/serverd.dir/depend:
	cd /root/workspace/GameServer/10_01_game/src/tools/robots && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/GameServer/10_01_game/src/tools/robots /root/workspace/GameServer/10_01_game/src/tools/robots /root/workspace/GameServer/10_01_game/src/tools/robots /root/workspace/GameServer/10_01_game/src/tools/robots /root/workspace/GameServer/10_01_game/src/tools/robots/CMakeFiles/serverd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/serverd.dir/depend
