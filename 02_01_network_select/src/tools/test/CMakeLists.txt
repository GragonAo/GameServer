cmake_minimum_required(VERSION 2.8)  # 设置所需的最低CMake版本

# 根据构建类型设置项目名称
if (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(MyProjectName test)  # 如果是Release版本，项目名称为test
else()
    set(CMAKE_BUILD_TYPE Debug)  # 否则，设置为Debug版本
    set(MyProjectName testd)  # Debug版本的项目名称为testd
endif()

# 输出一些调试信息
message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})  # 设置项目名称

# 查找当前目录下的所有源文件并将它们保存到SRCS变量中
aux_source_directory(. SRCS)
# 查找../../libs/network目录下的所有源文件并将它们保存到SRCS变量中
aux_source_directory(../../libs/network SRCS)

include_directories(../../libs)  # 包含头文件目录

# 设置生成的可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread")  # 通用编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")  # Debug版本编译选项
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")  # Release版本编译选项

link_directories(${CMAKE_BINARY_DIR}/../../../libs)  # 链接库文件目录

# 添加可执行文件，并将源文件列表SRCS传递给add_executable
add_executable(${MyProjectName} ${SRCS})
