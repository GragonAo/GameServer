cmake_minimum_required(VERSION 2.8)

set(MyProjectName allinone)

project(${MyProjectName})

aux_source_directory(. SRCS)
aux_source_directory(../login SRCS)
list(REMOVE_ITEM SRCS "../login/main.cpp")
aux_source_directory(../dbmgr SRCS)
list(REMOVE_ITEM SRCS "../dbmgr/main.cpp")

aux_source_directory(../../libs/libserver SRCS)
aux_source_directory(../../libs/libserver/protobuf SRCS)
set(SRCS ${SRCS} ../../tools/robots/robot_test.cpp) 

include_directories(../../tools/)
include_directories(../../libs)
include_directories(../../apps)
include_directories(/usr/local/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../../bin")

set(CMAKE_CXX_FLAGS "-Wall -std=c++14 -pthread -lprotobuf -lcurl -ljsoncpp -lyaml-cpp -llog4cplus -DEPOLL -D_DEBUG -O0 -g -ggdb")

add_executable(${MyProjectName} ${SRCS})
target_link_libraries(${MyProjectName} -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lssl -lcrypto -ldl) # mysql
