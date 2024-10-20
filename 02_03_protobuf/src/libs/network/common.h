#pragma once

#include <limits.h>
#include "protobuf/msg.pb.h"
#include "protobuf/proto_id.pb.h"

#ifndef WIN32
#define SOCKET int
#define INVALID_SOCKET -1
#endif

// 绑定一个带有4个参数的成员函数到一个对象上
// _self 表示对象指针，_f 表示成员函数指针
#define BindFunP4(_self, _f) std::bind(_f, _self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4)

// 绑定一个带有3个参数的成员函数到一个对象上
#define BindFunP3(_self, _f) std::bind(_f, _self, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

// 绑定一个带有2个参数的成员函数到一个对象上
#define BindFunP2(_self, _f) std::bind(_f, _self, std::placeholders::_1, std::placeholders::_2)

// 绑定一个带有1个参数的成员函数到一个对象上
#define BindFunP1(_self, _f) std::bind(_f, _self, std::placeholders::_1)

// 绑定一个不带参数的成员函数到一个对象上
#define BindFunP0(_self, _f) std::bind(_f, _self)
