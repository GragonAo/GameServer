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
CMAKE_SOURCE_DIR = /root/workspace/GameServer/02_02_network_epoll/src/libs/network

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/GameServer/02_02_network_epoll/src/libs/network

# Include any dependencies generated for this target.
include CMakeFiles/networkd.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/networkd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/networkd.dir/flags.make

CMakeFiles/networkd.dir/base_buffer.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/base_buffer.cpp.o: base_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/networkd.dir/base_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/base_buffer.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/base_buffer.cpp

CMakeFiles/networkd.dir/base_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/base_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/base_buffer.cpp > CMakeFiles/networkd.dir/base_buffer.cpp.i

CMakeFiles/networkd.dir/base_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/base_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/base_buffer.cpp -o CMakeFiles/networkd.dir/base_buffer.cpp.s

CMakeFiles/networkd.dir/connect_obj.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/connect_obj.cpp.o: connect_obj.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/networkd.dir/connect_obj.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/connect_obj.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/connect_obj.cpp

CMakeFiles/networkd.dir/connect_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/connect_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/connect_obj.cpp > CMakeFiles/networkd.dir/connect_obj.cpp.i

CMakeFiles/networkd.dir/connect_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/connect_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/connect_obj.cpp -o CMakeFiles/networkd.dir/connect_obj.cpp.s

CMakeFiles/networkd.dir/network.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/network.cpp.o: network.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/networkd.dir/network.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/network.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network.cpp

CMakeFiles/networkd.dir/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/network.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network.cpp > CMakeFiles/networkd.dir/network.cpp.i

CMakeFiles/networkd.dir/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/network.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network.cpp -o CMakeFiles/networkd.dir/network.cpp.s

CMakeFiles/networkd.dir/network_buffer.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/network_buffer.cpp.o: network_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/networkd.dir/network_buffer.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/network_buffer.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_buffer.cpp

CMakeFiles/networkd.dir/network_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/network_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_buffer.cpp > CMakeFiles/networkd.dir/network_buffer.cpp.i

CMakeFiles/networkd.dir/network_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/network_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_buffer.cpp -o CMakeFiles/networkd.dir/network_buffer.cpp.s

CMakeFiles/networkd.dir/network_connector.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/network_connector.cpp.o: network_connector.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/networkd.dir/network_connector.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/network_connector.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_connector.cpp

CMakeFiles/networkd.dir/network_connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/network_connector.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_connector.cpp > CMakeFiles/networkd.dir/network_connector.cpp.i

CMakeFiles/networkd.dir/network_connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/network_connector.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_connector.cpp -o CMakeFiles/networkd.dir/network_connector.cpp.s

CMakeFiles/networkd.dir/network_listen.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/network_listen.cpp.o: network_listen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/networkd.dir/network_listen.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/network_listen.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_listen.cpp

CMakeFiles/networkd.dir/network_listen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/network_listen.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_listen.cpp > CMakeFiles/networkd.dir/network_listen.cpp.i

CMakeFiles/networkd.dir/network_listen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/network_listen.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/network_listen.cpp -o CMakeFiles/networkd.dir/network_listen.cpp.s

CMakeFiles/networkd.dir/packet.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/packet.cpp.o: packet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/networkd.dir/packet.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/packet.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/packet.cpp

CMakeFiles/networkd.dir/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/packet.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/packet.cpp > CMakeFiles/networkd.dir/packet.cpp.i

CMakeFiles/networkd.dir/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/packet.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/packet.cpp -o CMakeFiles/networkd.dir/packet.cpp.s

CMakeFiles/networkd.dir/thread.cpp.o: CMakeFiles/networkd.dir/flags.make
CMakeFiles/networkd.dir/thread.cpp.o: thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/networkd.dir/thread.cpp.o"
	/usr/local/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/networkd.dir/thread.cpp.o -c /root/workspace/GameServer/02_02_network_epoll/src/libs/network/thread.cpp

CMakeFiles/networkd.dir/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/networkd.dir/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/02_02_network_epoll/src/libs/network/thread.cpp > CMakeFiles/networkd.dir/thread.cpp.i

CMakeFiles/networkd.dir/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/networkd.dir/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/02_02_network_epoll/src/libs/network/thread.cpp -o CMakeFiles/networkd.dir/thread.cpp.s

# Object files for target networkd
networkd_OBJECTS = \
"CMakeFiles/networkd.dir/base_buffer.cpp.o" \
"CMakeFiles/networkd.dir/connect_obj.cpp.o" \
"CMakeFiles/networkd.dir/network.cpp.o" \
"CMakeFiles/networkd.dir/network_buffer.cpp.o" \
"CMakeFiles/networkd.dir/network_connector.cpp.o" \
"CMakeFiles/networkd.dir/network_listen.cpp.o" \
"CMakeFiles/networkd.dir/packet.cpp.o" \
"CMakeFiles/networkd.dir/thread.cpp.o"

# External object files for target networkd
networkd_EXTERNAL_OBJECTS =

/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/base_buffer.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/connect_obj.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/network.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/network_buffer.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/network_connector.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/network_listen.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/packet.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/thread.cpp.o
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/build.make
/root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a: CMakeFiles/networkd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX static library /root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a"
	$(CMAKE_COMMAND) -P CMakeFiles/networkd.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/networkd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/networkd.dir/build: /root/workspace/GameServer/02_02_network_epoll/libs/libnetworkd.a

.PHONY : CMakeFiles/networkd.dir/build

CMakeFiles/networkd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/networkd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/networkd.dir/clean

CMakeFiles/networkd.dir/depend:
	cd /root/workspace/GameServer/02_02_network_epoll/src/libs/network && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/GameServer/02_02_network_epoll/src/libs/network /root/workspace/GameServer/02_02_network_epoll/src/libs/network /root/workspace/GameServer/02_02_network_epoll/src/libs/network /root/workspace/GameServer/02_02_network_epoll/src/libs/network /root/workspace/GameServer/02_02_network_epoll/src/libs/network/CMakeFiles/networkd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/networkd.dir/depend

