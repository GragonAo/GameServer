#include "socket_object.h"

// 定义一个全局的静态变量，表示无效的 SocketKey
SocketKey SocketKey::None = SocketKey(INVALID_SOCKET, NetworkType::None);

// 构造函数，用于初始化 SocketKey 对象
SocketKey::SocketKey(SOCKET socket, NetworkType netType)
{
    // 将传入的 socket 和网络类型赋值给成员变量
    Socket = socket;
    NetType = netType;
}

// 清理函数，将 SocketKey 对象的成员变量重置为无效状态
void SocketKey::Clean()
{
    // 将 Socket 重置为 INVALID_SOCKET，表示无效
    Socket = INVALID_SOCKET;
    // 将 NetType 重置为 NetworkType::None，表示无网络类型
    NetType = NetworkType::None;
}

// 清理函数，用于重置 ObjectKeyValue 对象
void ObjectKeyValue::Clean()
{
    // 将 KeyInt64 重置为 0
    KeyInt64 = 0;
    // 将 KeyStr 重置为空字符串
    KeyStr = "";
}

// 从 Protobuf 格式的网络对象键解析并设置 ObjectKey 的值
void ObjectKey::ParseFromProto(Proto::NetworkObjectKey protoKey)
{
    // 如果 Protobuf 键类型为 App 类型，则从 Protobuf 中提取 KeyInt64
    if (protoKey.key_type() == Proto::NetworkObjectKeyType::ObjectKeyTypeApp)
    {
        KeyType = ObjectKeyType::App; // 设置键类型为 App
        KeyValue.KeyInt64 = protoKey.key_value().key_int64(); // 设置 64 位整数键值
        KeyValue.KeyStr = ""; // 字符串键值清空
    }
    // 如果 Protobuf 键类型为 Account 类型，则从 Protobuf 中提取 KeyStr
    else if (protoKey.key_type() == Proto::NetworkObjectKeyType::ObjectKeyTypeAccount)
    {
        KeyType = ObjectKeyType::Account; // 设置键类型为 Account
        KeyValue.KeyInt64 = 0; // 整数键值重置为 0
        KeyValue.KeyStr = protoKey.key_value().key_str(); // 设置字符串键值
    }
    // 其他情况则为无效类型
    else
    {
        KeyType = ObjectKeyType::None; // 设置键类型为 None
        KeyValue.KeyInt64 = 0; // 整数键值重置为 0
        KeyValue.KeyStr = ""; // 字符串键值清空
    }
}

// 将 ObjectKey 的值序列化到 Protobuf 对象中
void ObjectKey::SerializeToProto(Proto::NetworkObjectKey* pProto) const
{
    // 将 ObjectKey 的类型转换并设置为 Protobuf 对象的类型
    pProto->set_key_type((Proto::NetworkObjectKeyType)KeyType);
    // 获取 Protobuf 对象中的 key_value 字段并设置其中的值
    auto pKeyValue = pProto->mutable_key_value();
    pKeyValue->set_key_int64(KeyValue.KeyInt64); // 设置 64 位整数键值
    pKeyValue->set_key_str(KeyValue.KeyStr.c_str()); // 设置字符串键值
}

// 清理函数，重置 ObjectKey 对象
void ObjectKey::Clean()
{
    KeyType = ObjectKeyType::None; // 设置键类型为 None
    KeyValue.Clean(); // 调用 ObjectKeyValue 的清理函数
}

// 构造函数，用于初始化 NetworkIdentify 对象，传入 SocketKey 和 ObjectKey
NetworkIdentify::NetworkIdentify(SocketKey socketKey, ObjectKey objKey)
{
    _socketKey = socketKey; // 将传入的 SocketKey 赋值给成员变量
    _objKey = objKey; // 将传入的 ObjectKey 赋值给成员变量
}

// 重载 log4cplus 的输出运算符，用于输出 NetworkIdentify 对象的详细信息
log4cplus::tostream& operator<<(log4cplus::tostream& os, const NetworkIdentify* pIdentify)
{
    // 输出 socket 和网络类型的基本信息
    os << " socket:" << pIdentify->GetSocketKey().Socket << " networkType:" << GetNetworkTypeName(pIdentify->GetSocketKey().NetType)
       << " connect type:" << GetConnectKeyTypeName(pIdentify->GetObjectKey().KeyType) << " value:";

    // 根据键类型输出对应的键值
    if (pIdentify->GetObjectKey().KeyType == ObjectKeyType::Account)
        os << pIdentify->GetObjectKey().KeyValue.KeyStr.c_str(); // 输出字符串键值
    else
        os << pIdentify->GetObjectKey().KeyValue.KeyInt64; // 输出整数键值

    return os; // 返回流对象
}

// 重载标准输出流的输出运算符，用于输出 NetworkIdentify 对象的详细信息
std::ostream& operator<<(std::ostream& os, NetworkIdentify* pIdentify)
{
    // 输出 socket 和网络类型的基本信息
    os << " socket:" << pIdentify->GetSocketKey().Socket
       << " networkType:" << GetNetworkTypeName(pIdentify->GetSocketKey().NetType)
       << " connect type:" << GetConnectKeyTypeName(pIdentify->GetObjectKey().KeyType) << " value:";

    // 根据键类型输出对应的键值
    if (pIdentify->GetObjectKey().KeyType == ObjectKeyType::Account)
        os << pIdentify->GetObjectKey().KeyValue.KeyStr.c_str(); // 输出字符串键值
    else
        os << pIdentify->GetObjectKey().KeyValue.KeyInt64; // 输出整数键值

    return os; // 返回流对象
}
