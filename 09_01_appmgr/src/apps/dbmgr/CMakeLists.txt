cmake_minimum_required(VERSION 2.8)

set(MyProjectName dbmgr)

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(../../libs/libserver SRCS)
aux_source_directory(../../libs/libserver/protobuf SRCS)
aux_source_directory(../../libs/libserver/mongoose SRCS)

include_directories(../../libs)
include_directories(/usr/local/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../../bin")

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lyaml-cpp -llog4cplus -DEPOLL -Wno-unknown-pragmas -g")

add_executable(${MyProjectName} ${SRCS})

target_link_libraries(${MyProjectName} -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lssl -lcrypto -ldl) # mysql
