#pragma once
#include "common.h"
#include "app_type.h"
#include "entity.h"

class Packet; // 前向声明 Packet 类

// AppInfo 结构体，存储应用程序信息
struct AppInfo
{
    APP_TYPE AppType;   // 应用程序类型
    int AppId;          // 应用程序 ID
    std::string Ip;     // 应用程序 IP 地址
    int Port;           // 应用程序端口号
    int Online;         // 在线人数
    SOCKET Socket;      // 应用程序的 Socket 连接

    // 从 Proto::AppInfoSync 解析信息并填充 AppInfo 对象
    bool Parse(Proto::AppInfoSync proto);
};

// SyncComponent 类，用于处理应用程序信息的同步
class SyncComponent
{
public:
    // 处理应用程序信息同步的网络数据包
    void AppInfoSyncHandle(Packet* pPacket);

    // 根据应用程序类型获取一个应用程序的信息
    bool GetOneApp(APP_TYPE appType, AppInfo* pInfo);

    // 显示当前同步的所有应用程序信息
    void CmdShow();

protected:
    // 解析来自网络的 Proto::AppInfoSync 消息，并更新应用程序信息
    void Parse(Proto::AppInfoSync proto, SOCKET socket);

protected:
    // 存储应用程序信息，使用 appId 作为键
    std::map<int, AppInfo> _apps;
};
