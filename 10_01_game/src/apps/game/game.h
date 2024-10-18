#pragma once
#include "libserver/thread_mgr.h"
#include "world_proxy_gather.h"
#include "lobby.h"
#include "console_cmd_world_proxy.h"

// 定义一个内联函数，用于初始化游戏相关组件
inline void InitializeComponentGame(ThreadMgr* pThreadMgr)
{
    // 创建 Lobby 组件，用于处理游戏大厅的逻辑
    pThreadMgr->CreateComponent<Lobby>();

    // 创建 WorldProxyGather 组件，负责代理和管理与世界相关的功能
    pThreadMgr->CreateComponent<WorldProxyGather>();

    // 从线程管理器获取控制台组件（Console），并注册一个新的控制台命令 "wproxy"
    auto pConsole = pThreadMgr->GetEntitySystem()->GetComponent<Console>();
    pConsole->Register<ConsoleCmdWorldProxy>("wproxy");
}
