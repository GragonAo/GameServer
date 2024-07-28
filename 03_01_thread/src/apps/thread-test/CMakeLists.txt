# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 2.8)

# 根据构建类型设置项目名称
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(MyProjectName thread-test)
else()
    set(CMAKE_BUILD_TYPE Debug)
    set(MyProjectName thread-testd)
endif()

# 输出一些状态信息，帮助调试
message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")  # 这里有个拼写错误，应为 CMAKE_BINARY_DIR
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "MyProjectName:${MyProjectName}")

# 定义项目名称
project(${MyProjectName})

# 添加当前目录下的所有源文件到变量SRCS中
aux_source_directory(. SRCS)

# 包含目录
include_directories(../../libs)
include_directories(/usr/include)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")  # 这里有个拼写错误，应为 -O3

# 指定链接库目录
link_directories(/usr/local/lib)
link_directories(${CMAKE_BINARY_DIR}/../../../libs)

# 定义可执行目标
add_executable(${MyProjectName} ${SRCS})
