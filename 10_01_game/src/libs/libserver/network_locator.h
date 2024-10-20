#pragma once
#include "network.h"        // 引入网络相关定义
#include "system.h"         // 引入系统定义
#include "app_type.h"       // 引入应用程序类型定义
#include "socket_object.h"  // 引入Socket对象定义

// NetworkLocator 类用于管理网络连接和定位，继承自 Entity 和 IAwakeSystem
class NetworkLocator : public Entity<NetworkLocator>, public IAwakeSystem<>
{
public:
    // 唤醒函数，重载自 IAwakeSystem
    void Awake() override;

    // 将对象放回对象池，清理资源
    void BackToPool() override;

    // 添加连接器网络定位器
    void AddConnectorLocator(INetwork* pNetwork, NetworkType networkType);

    // 根据SocketKey和ObjectKey添加网络标识
    void AddNetworkIdentify(SocketKey socket, ObjectKey objKey);

    // 根据应用程序类型获取相关的网络标识列表
    std::list<NetworkIdentify> GetAppNetworks(const APP_TYPE appType);

    // 根据应用程序Key移除网络标识
    void RemoveNetworkIdentify(uint64 appKey);

    // 获取连接器（INetwork实例），根据网络类型
    INetwork* GetConnector(NetworkType networkType);

    // 根据应用程序类型和应用程序ID获取网络标识
    NetworkIdentify GetNetworkIdentify(const APP_TYPE appType, const int appId);

    // 添加监听器网络定位器
    void AddListenLocator(INetwork* pNetwork, NetworkType networkType);

    // 根据网络类型获取监听器（INetwork实例）
    INetwork* GetListen(NetworkType networkType);

protected:
    // 处理应用程序注册的消息包
    void HandleAppRegister(Packet* pPacket);

private:
    std::mutex _lock;  // 用于多线程访问时的互斥锁

    // 存储应用程序类型和ID的网络标识 <apptype + appId, NetworkIdentify>
    std::map<uint64, NetworkIdentify> _netIdentify;

    // 存储连接器，按网络类型分类
    std::map<NetworkType, INetwork*> _connectors;

    // 存储监听器，按网络类型分类
    // 同时可以存在多个不同类型的监听器，如TCP和HTTP
    std::map<NetworkType, INetwork*> _listens;
};
