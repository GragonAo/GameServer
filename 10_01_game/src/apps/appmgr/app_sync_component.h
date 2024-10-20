#pragma once

#include "libserver/sync_component.h"  // 同步组件类的定义
#include "libserver/system.h"          // 系统相关的基类和接口
#include <json/writer.h>               // 用于处理 JSON 格式的数据

class AppSyncComponent : public Entity<AppSyncComponent>, public SyncComponent, public IAwakeSystem<>
{
public:
    // 重写 Awake 方法，组件初始化时调用
    void Awake() override;

    // 重写 BackToPool 方法，用于资源清理，返回对象池
    void BackToPool() override;

protected:
    // 处理应用程序命令
    void HandleCmdApp(Packet* pPacket);

    // 处理 HTTP 请求登录
    void HandleHttpRequestLogin(Packet* pPacket);

    // 处理应用信息同步
    void HandleAppInfoSync(Packet* pPacket);

private:
    // 同步游戏信息到登录服务器
    void SyncGameInfoToLogin();

private:
    // 用于 JSON 格式的输出
    Json::StreamWriter* _jsonWriter{ nullptr };
};
