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
#define engine_strncpy(dest, destsz, src, srcsz) strncpy(dest, src, srcsz)

#define SOCKET int
#define INVALID_SOCKET -1

typedef uint64_t uint64;
typedef uint32_t uint32;


//定义回调函数
#define BindFunP4(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4)
#define BindFunP3(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
#define BindFunP2(_self,_f) std:: bind(_f,_self,std::placeholders::_1,std::placeholders::_2)
#define BindFunP1(_self,_f) std:: bind(_f,_self,std::placeholders::_1)
#define BindFunP0(_self, _f) std::bind ( _f, _self)
