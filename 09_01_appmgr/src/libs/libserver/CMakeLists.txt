cmake_minimum_required(VERSION 2.8)
set(MyProjectName server)
message(STATUS "ProjectName:${MyProjectName}")

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(./protobuf SRCS)
aux_source_directory(./mongoose SRCS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../../../../libs")

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lyaml-cpp -DEPOLL -O3 -DNDEBUG")

add_library(${MyProjectName} STATIC ${SRCS})