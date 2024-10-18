#pragma once

#include "network.h"

// NetworkListen 继承自 Network 类，同时实现了两个 IAwakeSystem 接口，
// 提供了监听连接、处理新连接等功能
class NetworkListen : public Network, public IAwakeSystem<std::string, int>, public IAwakeSystem<int, int>
{
public:
    // 重写 Awake 方法，初始化监听器
    // 使用 IP 和端口号设置监听器
    void Awake(std::string ip, int port) override;

    // 重写 Awake 方法，通过应用类型和应用 ID 初始化
    void Awake(int appType, int appId) override;

    // 额外的 Awake 重载方法，接受 IP、端口号和网络类型进行初始化
    void Awake(std::string ip, int port, NetworkType iType);

    // 将当前对象放回对象池，释放资源并重置状态
    void BackToPool() override;

    // 定期更新监听状态，处理新连接和消息
    virtual void Update();

    // 返回类型名称，用于调试和反射
    const char* GetTypeName() override;

    // 返回类型的哈希码，用于唯一标识类型
    uint64 GetTypeHashCode() override;

    // 显示当前监听器的状态和配置信息
    void CmdShow();

    // 判断是否为单例模式，返回 true 说明该类是单例模式
    static bool IsSingle() { return true; }

private:
    // 处理特定的监听键，处理新到达的连接请求
    void HandleListenKey(Packet* pPacket);

protected:
    // 接受新的连接请求，并返回新连接的 socket 文件描述符
    virtual int Accept();

#ifdef EPOLL
    // 处理 epoll 事件，监听 socket 文件描述符的变化
    virtual void OnEpoll(SOCKET fd, int index) override;
#endif

private:
    // epoll 模式下，主 socket 事件的索引，初始化为 -1 表示未设置
#ifdef EPOLL
    int _mainSocketEventIndex{ -1 };
#endif

    // 主监听 socket，初始化为 INVALID_SOCKET 表示无效的 socket
    SOCKET _masterSocket{ INVALID_SOCKET };
};
