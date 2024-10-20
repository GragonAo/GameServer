#include "libserver/common.h"           // 包含通用定义和全局函数
#include "libserver/server_app.h"        // 包含服务器应用程序相关的类定义
#include "libserver/global.h"            // 包含全局配置信息
#include "libserver/network_listen.h"    // 包含网络监听组件的类定义

#include "space.h"                       // 包含 Space 相关的功能定义

int main(int argc, char *argv[])
{
    // 1. 设置当前应用的类型为 APP_SPACE，用于标识应用的种类
    const APP_TYPE curAppType = APP_TYPE::APP_SPACE;

    // 2. 创建一个 ServerApp 实例，传入应用类型和命令行参数，并进行初始化
    ServerApp app(curAppType, argc, argv);
    app.Initialize();

    // 3. 获取线程管理器的实例，用于管理各个组件的运行
    auto pThreadMgr = ThreadMgr::GetInstance();

    // 4. 初始化空间组件（Space），这是游戏世界或逻辑空间的管理模块
    InitializeComponentSpace(pThreadMgr);

    // 5. 获取全局的配置信息
    const auto pGlobal = Global::GetInstance();

    // 6. 创建网络监听组件，监听来自客户端或其他服务器的连接
    //    这里指定了监听线程（ListenThread）和应用类型以及应用 ID
    pThreadMgr->CreateComponent<NetworkListen>(ListenThread, false, (int)pGlobal->GetCurAppType(), pGlobal->GetCurAppId());

    // 7. 运行应用程序的主循环
    app.Run();

    // 8. 退出前进行资源清理
    app.Dispose();

    return 0;
}
