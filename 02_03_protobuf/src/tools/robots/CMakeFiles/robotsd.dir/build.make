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
CMAKE_SOURCE_DIR = /root/workspace/GameServer/02_03_protobuf/src/tools/robots

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/GameServer/02_03_protobuf/src/tools/robots

# Include any dependencies generated for this target.
include CMakeFiles/robotsd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/robotsd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/robotsd.dir/flags.make

CMakeFiles/robotsd.dir/client.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/client.cpp.o: client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/robotsd.dir/client.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/client.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client.cpp

CMakeFiles/robotsd.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/client.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client.cpp > CMakeFiles/robotsd.dir/client.cpp.i

CMakeFiles/robotsd.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/client.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client.cpp -o CMakeFiles/robotsd.dir/client.cpp.s

CMakeFiles/robotsd.dir/client_thread.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/client_thread.cpp.o: client_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/robotsd.dir/client_thread.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/client_thread.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client_thread.cpp

CMakeFiles/robotsd.dir/client_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/client_thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client_thread.cpp > CMakeFiles/robotsd.dir/client_thread.cpp.i

CMakeFiles/robotsd.dir/client_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/client_thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/tools/robots/client_thread.cpp -o CMakeFiles/robotsd.dir/client_thread.cpp.s

CMakeFiles/robotsd.dir/robots.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/robots.cpp.o: robots.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/robotsd.dir/robots.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/robots.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/tools/robots/robots.cpp

CMakeFiles/robotsd.dir/robots.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/robots.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/tools/robots/robots.cpp > CMakeFiles/robotsd.dir/robots.cpp.i

CMakeFiles/robotsd.dir/robots.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/robots.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/tools/robots/robots.cpp -o CMakeFiles/robotsd.dir/robots.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.s

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o: CMakeFiles/robotsd.dir/flags.make
CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o: /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o -c /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc > CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.i

CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc -o CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.s

# Object files for target robotsd
robotsd_OBJECTS = \
"CMakeFiles/robotsd.dir/client.cpp.o" \
"CMakeFiles/robotsd.dir/client_thread.cpp.o" \
"CMakeFiles/robotsd.dir/robots.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o" \
"CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o"

# External object files for target robotsd
robotsd_EXTERNAL_OBJECTS =

/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/client.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/client_thread.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/robots.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/base_buffer.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/connect_obj.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_buffer.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_connector.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/network_listen.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/packet.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/thread.cpp.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/msg.pb.cc.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/root/workspace/GameServer/02_03_protobuf/src/libs/network/protobuf/proto_id.pb.cc.o
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/build.make
/root/workspace/GameServer/02_03_protobuf/bin/robotsd: CMakeFiles/robotsd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable /root/workspace/GameServer/02_03_protobuf/bin/robotsd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/robotsd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/robotsd.dir/build: /root/workspace/GameServer/02_03_protobuf/bin/robotsd

.PHONY : CMakeFiles/robotsd.dir/build

CMakeFiles/robotsd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/robotsd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/robotsd.dir/clean

CMakeFiles/robotsd.dir/depend:
	cd /root/workspace/GameServer/02_03_protobuf/src/tools/robots && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/GameServer/02_03_protobuf/src/tools/robots /root/workspace/GameServer/02_03_protobuf/src/tools/robots /root/workspace/GameServer/02_03_protobuf/src/tools/robots /root/workspace/GameServer/02_03_protobuf/src/tools/robots /root/workspace/GameServer/02_03_protobuf/src/tools/robots/CMakeFiles/robotsd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/robotsd.dir/depend

