# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /root/workspace/GameServer/05_01_PF/src/libs/libserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/workspace/GameServer/05_01_PF/src/libs/libserver/Build

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/base_buffer.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/base_buffer.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/base_buffer.cpp
CMakeFiles/server.dir/base_buffer.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/base_buffer.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/base_buffer.cpp.o -MF CMakeFiles/server.dir/base_buffer.cpp.o.d -o CMakeFiles/server.dir/base_buffer.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/base_buffer.cpp

CMakeFiles/server.dir/base_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/base_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/base_buffer.cpp > CMakeFiles/server.dir/base_buffer.cpp.i

CMakeFiles/server.dir/base_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/base_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/base_buffer.cpp -o CMakeFiles/server.dir/base_buffer.cpp.s

CMakeFiles/server.dir/connect_obj.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/connect_obj.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/connect_obj.cpp
CMakeFiles/server.dir/connect_obj.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/server.dir/connect_obj.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/connect_obj.cpp.o -MF CMakeFiles/server.dir/connect_obj.cpp.o.d -o CMakeFiles/server.dir/connect_obj.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/connect_obj.cpp

CMakeFiles/server.dir/connect_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/connect_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/connect_obj.cpp > CMakeFiles/server.dir/connect_obj.cpp.i

CMakeFiles/server.dir/connect_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/connect_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/connect_obj.cpp -o CMakeFiles/server.dir/connect_obj.cpp.s

CMakeFiles/server.dir/console.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/console.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/console.cpp
CMakeFiles/server.dir/console.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/server.dir/console.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/console.cpp.o -MF CMakeFiles/server.dir/console.cpp.o.d -o CMakeFiles/server.dir/console.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/console.cpp

CMakeFiles/server.dir/console.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/console.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/console.cpp > CMakeFiles/server.dir/console.cpp.i

CMakeFiles/server.dir/console.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/console.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/console.cpp -o CMakeFiles/server.dir/console.cpp.s

CMakeFiles/server.dir/global.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/global.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/global.cpp
CMakeFiles/server.dir/global.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/server.dir/global.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/global.cpp.o -MF CMakeFiles/server.dir/global.cpp.o.d -o CMakeFiles/server.dir/global.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/global.cpp

CMakeFiles/server.dir/global.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/global.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/global.cpp > CMakeFiles/server.dir/global.cpp.i

CMakeFiles/server.dir/global.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/global.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/global.cpp -o CMakeFiles/server.dir/global.cpp.s

CMakeFiles/server.dir/message_list.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/message_list.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/message_list.cpp
CMakeFiles/server.dir/message_list.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/server.dir/message_list.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/message_list.cpp.o -MF CMakeFiles/server.dir/message_list.cpp.o.d -o CMakeFiles/server.dir/message_list.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/message_list.cpp

CMakeFiles/server.dir/message_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/message_list.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/message_list.cpp > CMakeFiles/server.dir/message_list.cpp.i

CMakeFiles/server.dir/message_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/message_list.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/message_list.cpp -o CMakeFiles/server.dir/message_list.cpp.s

CMakeFiles/server.dir/network.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/network.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/network.cpp
CMakeFiles/server.dir/network.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/server.dir/network.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/network.cpp.o -MF CMakeFiles/server.dir/network.cpp.o.d -o CMakeFiles/server.dir/network.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/network.cpp

CMakeFiles/server.dir/network.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/network.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/network.cpp > CMakeFiles/server.dir/network.cpp.i

CMakeFiles/server.dir/network.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/network.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/network.cpp -o CMakeFiles/server.dir/network.cpp.s

CMakeFiles/server.dir/network_buffer.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/network_buffer.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_buffer.cpp
CMakeFiles/server.dir/network_buffer.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/server.dir/network_buffer.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/network_buffer.cpp.o -MF CMakeFiles/server.dir/network_buffer.cpp.o.d -o CMakeFiles/server.dir/network_buffer.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_buffer.cpp

CMakeFiles/server.dir/network_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/network_buffer.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_buffer.cpp > CMakeFiles/server.dir/network_buffer.cpp.i

CMakeFiles/server.dir/network_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/network_buffer.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_buffer.cpp -o CMakeFiles/server.dir/network_buffer.cpp.s

CMakeFiles/server.dir/network_connector.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/network_connector.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_connector.cpp
CMakeFiles/server.dir/network_connector.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/server.dir/network_connector.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/network_connector.cpp.o -MF CMakeFiles/server.dir/network_connector.cpp.o.d -o CMakeFiles/server.dir/network_connector.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_connector.cpp

CMakeFiles/server.dir/network_connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/network_connector.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_connector.cpp > CMakeFiles/server.dir/network_connector.cpp.i

CMakeFiles/server.dir/network_connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/network_connector.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_connector.cpp -o CMakeFiles/server.dir/network_connector.cpp.s

CMakeFiles/server.dir/network_listen.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/network_listen.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_listen.cpp
CMakeFiles/server.dir/network_listen.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/server.dir/network_listen.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/network_listen.cpp.o -MF CMakeFiles/server.dir/network_listen.cpp.o.d -o CMakeFiles/server.dir/network_listen.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_listen.cpp

CMakeFiles/server.dir/network_listen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/network_listen.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_listen.cpp > CMakeFiles/server.dir/network_listen.cpp.i

CMakeFiles/server.dir/network_listen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/network_listen.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/network_listen.cpp -o CMakeFiles/server.dir/network_listen.cpp.s

CMakeFiles/server.dir/packet.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/packet.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/packet.cpp
CMakeFiles/server.dir/packet.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/server.dir/packet.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/packet.cpp.o -MF CMakeFiles/server.dir/packet.cpp.o.d -o CMakeFiles/server.dir/packet.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/packet.cpp

CMakeFiles/server.dir/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/packet.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/packet.cpp > CMakeFiles/server.dir/packet.cpp.i

CMakeFiles/server.dir/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/packet.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/packet.cpp -o CMakeFiles/server.dir/packet.cpp.s

CMakeFiles/server.dir/server_app.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/server_app.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/server_app.cpp
CMakeFiles/server.dir/server_app.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/server.dir/server_app.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/server_app.cpp.o -MF CMakeFiles/server.dir/server_app.cpp.o.d -o CMakeFiles/server.dir/server_app.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/server_app.cpp

CMakeFiles/server.dir/server_app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server_app.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/server_app.cpp > CMakeFiles/server.dir/server_app.cpp.i

CMakeFiles/server.dir/server_app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server_app.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/server_app.cpp -o CMakeFiles/server.dir/server_app.cpp.s

CMakeFiles/server.dir/thread.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/thread.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread.cpp
CMakeFiles/server.dir/thread.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/server.dir/thread.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/thread.cpp.o -MF CMakeFiles/server.dir/thread.cpp.o.d -o CMakeFiles/server.dir/thread.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread.cpp

CMakeFiles/server.dir/thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/thread.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread.cpp > CMakeFiles/server.dir/thread.cpp.i

CMakeFiles/server.dir/thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/thread.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread.cpp -o CMakeFiles/server.dir/thread.cpp.s

CMakeFiles/server.dir/thread_mgr.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/thread_mgr.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_mgr.cpp
CMakeFiles/server.dir/thread_mgr.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/server.dir/thread_mgr.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/thread_mgr.cpp.o -MF CMakeFiles/server.dir/thread_mgr.cpp.o.d -o CMakeFiles/server.dir/thread_mgr.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_mgr.cpp

CMakeFiles/server.dir/thread_mgr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/thread_mgr.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_mgr.cpp > CMakeFiles/server.dir/thread_mgr.cpp.i

CMakeFiles/server.dir/thread_mgr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/thread_mgr.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_mgr.cpp -o CMakeFiles/server.dir/thread_mgr.cpp.s

CMakeFiles/server.dir/thread_obj.cpp.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/thread_obj.cpp.o: /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_obj.cpp
CMakeFiles/server.dir/thread_obj.cpp.o: CMakeFiles/server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/server.dir/thread_obj.cpp.o"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/server.dir/thread_obj.cpp.o -MF CMakeFiles/server.dir/thread_obj.cpp.o.d -o CMakeFiles/server.dir/thread_obj.cpp.o -c /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_obj.cpp

CMakeFiles/server.dir/thread_obj.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/thread_obj.cpp.i"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_obj.cpp > CMakeFiles/server.dir/thread_obj.cpp.i

CMakeFiles/server.dir/thread_obj.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/thread_obj.cpp.s"
	/usr/local/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/workspace/GameServer/05_01_PF/src/libs/libserver/thread_obj.cpp -o CMakeFiles/server.dir/thread_obj.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/base_buffer.cpp.o" \
"CMakeFiles/server.dir/connect_obj.cpp.o" \
"CMakeFiles/server.dir/console.cpp.o" \
"CMakeFiles/server.dir/global.cpp.o" \
"CMakeFiles/server.dir/message_list.cpp.o" \
"CMakeFiles/server.dir/network.cpp.o" \
"CMakeFiles/server.dir/network_buffer.cpp.o" \
"CMakeFiles/server.dir/network_connector.cpp.o" \
"CMakeFiles/server.dir/network_listen.cpp.o" \
"CMakeFiles/server.dir/packet.cpp.o" \
"CMakeFiles/server.dir/server_app.cpp.o" \
"CMakeFiles/server.dir/thread.cpp.o" \
"CMakeFiles/server.dir/thread_mgr.cpp.o" \
"CMakeFiles/server.dir/thread_obj.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/base_buffer.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/connect_obj.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/console.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/global.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/message_list.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/network.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/network_buffer.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/network_connector.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/network_listen.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/packet.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/server_app.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/thread.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/thread_mgr.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/thread_obj.cpp.o
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/build.make
/root/workspace/GameServer/05_01_PF/libs/libserver.a: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX static library /root/workspace/GameServer/05_01_PF/libs/libserver.a"
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: /root/workspace/GameServer/05_01_PF/libs/libserver.a
.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /root/workspace/GameServer/05_01_PF/src/libs/libserver/Build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/workspace/GameServer/05_01_PF/src/libs/libserver /root/workspace/GameServer/05_01_PF/src/libs/libserver /root/workspace/GameServer/05_01_PF/src/libs/libserver/Build /root/workspace/GameServer/05_01_PF/src/libs/libserver/Build /root/workspace/GameServer/05_01_PF/src/libs/libserver/Build/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

