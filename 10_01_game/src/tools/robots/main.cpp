#include "libserver/common.h"
#include "libserver/server_app.h"
#include "robot_mgr.h"
#include "robot_console_login.h"
#include "global_robots.h"
#include "robot_console_http.h"
#include "libserver/network_connector.h"
#include "robot_collection.h"

int main(int argc, char* argv[])
{
    // 单例模式初始化 GlobalRobots，用于管理机器人数量
    GlobalRobots::Instance();

    // 设置当前应用类型为机器人应用
    const APP_TYPE curAppType = APP_TYPE::APP_ROBOT;

    // 初始化服务器应用程序
    ServerApp app(curAppType, argc, argv);
    app.Initialize();

    // 获取线程管理器的实例
    const auto pThreadMgr = ThreadMgr::GetInstance();

    // 获取控制台组件，并注册两个控制台命令：login 和 http
    auto pConsole = pThreadMgr->GetEntitySystem()->GetComponent<Console>();
    pConsole->Register<RobotConsoleLogin>("login");
    pConsole->Register<RobotConsoleHttp>("http");

    // 向线程管理系统添加机器人管理组件（RobotMgr）
    pThreadMgr->GetEntitySystem()->AddComponent<RobotMgr>();

    // 创建 TCP 和 HTTP 的网络连接器组件，用于连接到相应服务器
    pThreadMgr->CreateComponent<NetworkConnector>(ConnectThread, false, (int)NetworkType::TcpConnector, 0);
    pThreadMgr->CreateComponent<NetworkConnector>(ConnectThread, false, (int)NetworkType::HttpConnector, 0);

    pThreadMgr->CreateComponent<RobotCollection>(LogicThread, true);
    pThreadMgr->GetEntitySystem()->AddComponent<RobotCollection>();

    // 运行服务器应用程序，进入主循环
    app.Run();

    // 应用程序退出时，清理资源
    app.Dispose();
}
