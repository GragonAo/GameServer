cmake_minimum_required(VERSION 2.8)

set(MyProjectName login)

message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(../../libs/libserver SRCS)
aux_source_directory(../../libs/libserver/protobuf SRCS)

include_directories(../../libs)
include_directories(../../../../include/common)
include_directories(/usr/local/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../../bin")

# 设置调试和发布配置
# set(CMAKE_CXX_FLAGS_DEBUG "-Wall -std=c++14 -pthread -lprotobuf -lcurl -ljsoncpp -DEPOLL -g")
# set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lcurl -ljsoncpp -DEPOLL -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lcurl -ljsoncpp -DEPOLL -g")

# # 默认构建类型
# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Debug)
# endif()

add_executable(${MyProjectName} ${SRCS})
