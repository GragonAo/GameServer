cmake_minimum_required(VERSION 2.8)

set(MyProjectName robots)

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(../../libs/libserver SRCS)
aux_source_directory(../../libs/libserver/protobuf SRCS)

include_directories(../../libs)
include_directories(/usr/local/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../../bin")

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lyaml-cpp -DEPOLL -D_DEBUG -O0 -g -ggdb")
link_directories(${CMAKE_BINARY_DIR}/../../../libs)

add_executable(${MyProjectName} ${SRCS})