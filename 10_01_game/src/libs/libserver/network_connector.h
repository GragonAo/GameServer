#pragma once

#include <utility>
#include "network.h"
#include "connect_obj.h"
#include "app_type.h"

class Packet;

// `ConnectDetail` 结构体用于存储连接的详细信息，继承自 `SnObject` 和 `IDisposable`。
struct ConnectDetail : public SnObject, public IDisposable
{
public:
    // 构造函数，用于初始化连接的详细信息，包括 `ObjectKey`、IP 地址和端口号。
    ConnectDetail(ObjectKey key, std::string ip, int port)
    {
        Key = std::move(key);  // 移动赋值 ObjectKey
        Ip = std::move(ip);    // 移动赋值 IP 地址
        Port = port;           // 赋值端口号
    };

    // 虚函数 `Dispose`，当连接对象不再需要时调用，但此处没有具体的释放逻辑。
    void Dispose() override { }

    // IP 地址和端口号
    std::string Ip{ "" };  // 存储 IP 地址
    int Port{ 0 };         // 存储端口号

    // 用于唯一标识连接的 `ObjectKey`
    ObjectKey Key{ ObjectKeyType::None , { 0, ""} };
};

// `NetworkConnector` 类继承自 `Network` 和 `IAwakeSystem`，实现了连接管理的功能。
class NetworkConnector :
    public Network,
    public IAwakeSystem<int, int>  // 实现 `IAwakeSystem` 接口，用于初始化
{
public:
    // 重写 `Awake` 函数，初始化网络连接器
    void Awake(int iType, int mixConnectAppType) override;

    // 定期更新函数，用于处理网络状态更新
    virtual void Update();

    // 返回类的类型名称，用于调试
    const char* GetTypeName() override;

    // 返回类的类型哈希码，用于类型识别
    uint64 GetTypeHashCode() override;

    // 连接到指定的网络，传入 `ConnectDetail` 对象，返回是否连接成功
    bool Connect(ConnectDetail* pDetail);

private:
    // 处理网络连接的回调函数，处理与服务器之间的连接
    void HandleNetworkConnect(Packet* pPacket);

    // 创建连接器对象，指定应用类型、应用 ID、IP 地址和端口号
    void CreateConnector(APP_TYPE appType, int appId, std::string ip, int port);

private:
    // 用于存储等待连接的详细信息的缓存
    CacheRefresh<ConnectDetail> _connecting;
};
