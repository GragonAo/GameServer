cmake_minimum_required(VERSION 2.8)

# 设置项目名称
set(MyProjectName network)

# 根据构建类型设置项目名称和输出信息
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(MyProjectName network)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(MyProjectName network)
endif()

# 打印构建类型和项目名称
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName: ${MyProjectName}")

# 设置项目名称
project(${MyProjectName})

# 将当前目录下的所有源文件添加到变量 SRCS 中
aux_source_directory(. SRCS)

# 设置静态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../../libs")

# 设置编译器标志
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 添加静态库
add_library(${MyProjectName} STATIC ${SRCS})
