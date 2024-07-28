#pragma once

#include <climits>
#include <cstdint>
#include "protobuf/proto_id.pb.h"
#include "protobuf/msg.pb.h"

#define PLATFORM_UNIX 1

#define ENGINE_PLATFORM RLATFORM_UNIX

#include <string.h>
#include <stdint.h>
#define engine_stricmp strcasecmp
#define engine_access access

#define SOCKET int
#define INVALID_SOCKET -1

typedef uint64_t uint64;
typedef uint32_t uint32;

enum APP_TYPE{
    APP_Listen = 0,
    
    APP_DB_MGR = 1,
    APP_GAME_MGR = 1<<1,
    APP_SPACE_MGR = 1 << 2,
    APP_LOGIN = 1<<3,
    APP_GAME  = 1<< 4,
    APP_SPACE = 1 << 5,
    APP_ROBOT = 1<< 6,

    APP_APPMGR  = APP_GAME_MGR | APP_SPACE_MGR,

    APP_ALL = APP_DB_MGR | APP_APPMGR | APP_LOGIN | APP_GAME | APP_SPACE
};

#define BindFunP4(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4)
#define BindFunP3(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
#define BindFunP2(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2)
#define BindFunP1(_self,_f) std:: bind(_f,_self,std::placeholders::_1)
