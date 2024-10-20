#pragma once

#include "common.h" // 包含常用的头文件
#include "network_type.h" // 包含网络类型的定义
#include <log4cplus/streams.h> // 包含 log4cplus 库的输出流支持
#include <ostream> // 包含输出流的支持

// SocketKey 结构，用于存储和标识套接字信息
struct SocketKey
{
    SocketKey(SOCKET socket, NetworkType netType); // 构造函数

    void Clean(); // 清理方法

    SOCKET Socket; // 套接字
    NetworkType NetType; // 网络类型

    // 不同于运算符
    bool operator != (const SocketKey other)
    {
        if (Socket != other.Socket)
            return false;

        if (NetType != other.NetType)
            return false;

        return true;
    };

    // 相等运算符
    bool operator == (const SocketKey other)
    {
        return (Socket == other.Socket) && (NetType == other.NetType);
    };

    static SocketKey None; // 表示无效的 SocketKey
};

// ObjectKeyType 枚举，表示对象的类型
enum class ObjectKeyType
{
    None = Proto::NetworkObjectKeyType::ObjectKeyTypeNone, // 无效类型
    Account = Proto::NetworkObjectKeyType::ObjectKeyTypeAccount, // 账户类型
    App = Proto::NetworkObjectKeyType::ObjectKeyTypeApp, // 应用类型
};

// 根据 ObjectKeyType 获取对应的字符串名称
inline const char* GetConnectKeyTypeName(const ObjectKeyType iType)
{
    if (iType == ObjectKeyType::Account)
        return "Account";
    else if (iType == ObjectKeyType::App)
        return "App";
    else
        return "None";
}

// ObjectKeyValue 结构，用于存储对象的键值信息
struct ObjectKeyValue
{
    uint64 KeyInt64{ 0 }; // 整数键
    std::string KeyStr{ "" }; // 字符串键

    void Clean(); // 清理方法

    // 不同于运算符
    bool operator != (const ObjectKeyValue other)
    {
        if (KeyInt64 != other.KeyInt64)
            return false;

        if (KeyStr != other.KeyStr)
            return false;

        return true;
    };

    // 相等运算符
    bool operator == (const ObjectKeyValue other)
    {
        return (KeyInt64 == other.KeyInt64) && (KeyStr == other.KeyStr);
    };
};

// ObjectKey 结构，用于存储对象的键信息
struct ObjectKey
{
    ObjectKeyType KeyType{ ObjectKeyType::None }; // 对象键类型
    ObjectKeyValue KeyValue{ 0, "" }; // 对象键值

    void ParseFromProto(Proto::NetworkObjectKey protoKey); // 从 Proto 解析
    void SerializeToProto(Proto::NetworkObjectKey* pProto) const; // 序列化到 Proto
    void Clean(); // 清理方法

    // 不同于运算符
    bool operator != (const ObjectKey other)
    {
        if (KeyType != other.KeyType)
            return false;

        if (KeyValue != other.KeyValue)
            return false;

        return true;
    };

    // 相等运算符
    bool operator == (const ObjectKey other)
    {
        return (KeyType == other.KeyType) && (KeyValue == other.KeyValue);
    };
};

// NetworkIdentify 结构，表示网络标识
struct NetworkIdentify
{
public:
    NetworkIdentify() = default; // 默认构造函数
    NetworkIdentify(SocketKey socketKey, ObjectKey objKey); // 带参数的构造函数

    virtual ~NetworkIdentify() = default; // 虚析构函数
    SocketKey GetSocketKey() const { return _socketKey; } // 获取 SocketKey
    ObjectKey GetObjectKey() const { return _objKey; } // 获取 ObjectKey

protected:
    SocketKey _socketKey{ INVALID_SOCKET, NetworkType::None }; // 存储的 SocketKey
    ObjectKey _objKey{ ObjectKeyType::None , {0, ""} }; // 存储的 ObjectKey
};

// 重载输出运算符，用于 log4cplus 输出
log4cplus::tostream& operator <<(log4cplus::tostream& os, const NetworkIdentify* pIdentify);

// 重载输出运算符，用于 std::ostream 输出
std::ostream& operator <<(std::ostream& os, NetworkIdentify* pIdentify);
