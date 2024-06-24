cmake_minimum_required(VERSION 2.8)

# 设置项目名称和库文件名
if (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(MyProjectName server)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(MyProjectName serverd)
endif()

set(MyLibNetWorkName network.a)

message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")
message(STATUS "NetworklibName:${MyLibNetWorkName}")

project(${MyProjectName})

aux_source_directory(. SRCS)

# 设置包含目录
include_directories(../../libs)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 设置链接目录
link_directories(${CMAKE_BINARY_DIR}/../../../libs)

# 添加可执行文件
add_executable(${MyProjectName} ${SRCS})

# 链接库文件
target_link_libraries(${MyProjectName} network)
