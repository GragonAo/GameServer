cmake_minimum_required(VERSION 2.8)

# 根据构建类型设置项目名称
if (CMAKE_BUILD_TYPE STREQUAL Release) 
    set(MyProjectName robots)  # 释放版本的项目名称
else()
    set(CMAKE_BUILD_TYPE Debug)  # 设置为调试版本
    set(MyProjectName robotsd)    # 调试版本的项目名称
endif()

# 打印一些状态信息
message(STATUS "CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")  # 输出二进制目录
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")  # 输出构建类型
message(STATUS "ProjectName:${MyProjectName}")          # 输出项目名称

# 定义项目
project(${MyProjectName})

# 自动收集当前目录及其他目录的源文件
aux_source_directory(. SRCS)                                     # 当前目录的源文件
aux_source_directory(../../libs/libserver SRCS)                 # libserver 目录的源文件
aux_source_directory(../../libs/libserver/protobuf SRCS)       # protobuf 目录的源文件
aux_source_directory(../../libs/libserver/mongoose SRCS)       # mongoose 目录的源文件
aux_source_directory(../../libs/libplayer SRCS)                # libplayer 目录的源文件

# 包含头文件目录
include_directories(../../libs)                                 # 包含 libs 目录
include_directories(../../../../include/common)                 # 包含 common 目录
include_directories(/usr/local/include)                         # 包含系统目录

# 设置可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin")

# 设置编译选项
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -ljsoncpp -lyaml-cpp -llog4cplus -DEPOLL")  # 一般编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -O0 -g -ggdb")   # 调试模式下的编译选项
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")           # 释放模式下的编译选项

# 链接库目录
link_directories(${CMAKE_BINARY_DIR}/../../../libs)          # 指定 libs 目录
link_directories(${CMAKE_BINARY_DIR}/../../../../libs/linux)  # 指定 Linux 目录

# 创建可执行文件
add_executable(${MyProjectName} ${SRCS})
# target_link_libraries(${MyProjectName} libserverd.a)  # 可选链接库（已注释）
