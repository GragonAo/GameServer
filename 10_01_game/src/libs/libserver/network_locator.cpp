#include "network_locator.h"
#include "network_listen.h"
#include <algorithm>
#include <utility>
#include "message_component.h"
#include "message_system_help.h"
#include "component_help.h"
#include "socket_object.h"
#include "global.h"

/**
 * @brief 初始化 NetworkLocator 对象，设置消息回调和注册处理函数。
 */
void NetworkLocator::Awake()
{
    std::lock_guard<std::mutex> g1(_lock);  // 加锁，保证线程安全

    // 清空网络标识和网络连接器、监听器的映射
    _netIdentify.clear();
    _connectors.clear();
    _listens.clear();

    // 创建消息回调函数对象
    auto pMsgCallBack = new MessageCallBackFunction();
    // 为当前实体添加消息组件，注册回调函数
    AddComponent<MessageComponent>(pMsgCallBack);

    // 注册处理函数，处理 MI_AppRegister 消息
    pMsgCallBack->RegisterFunction(Proto::MsgId::MI_AppRegister, BindFunP1(this, &NetworkLocator::HandleAppRegister));
}

/**
 * @brief 将 NetworkLocator 对象返回到对象池，并清理资源。
 */
void NetworkLocator::BackToPool()
{
    std::lock_guard<std::mutex> g1(_lock);  // 加锁，保证线程安全

    // 清空网络标识和网络连接器、监听器的映射
    _netIdentify.clear();
    _connectors.clear();
    _listens.clear();
}

/**
 * @brief 添加网络连接器定位器，映射网络类型到具体的网络连接器对象。
 * 
 * @param pNetwork 指向网络连接器对象的指针
 * @param networkType 网络类型（如 TcpConnector、HttpConnector）
 */
void NetworkLocator::AddConnectorLocator(INetwork* pNetwork, NetworkType networkType)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全
    _connectors[networkType] = pNetwork;       // 将网络类型与网络连接器对象关联
}

/**
 * @brief 添加网络标识，将 SocketKey 和 ObjectKey 映射到 NetworkIdentify 对象。
 * 
 * @param socket SocketKey 对象，包含套接字和网络类型
 * @param objKey ObjectKey 对象，包含对象类型和键值
 */
void NetworkLocator::AddNetworkIdentify(SocketKey socket, ObjectKey objKey)
{
    _lock.lock();  // 加锁，手动管理锁的粒度
    uint64 appKey = objKey.KeyValue.KeyInt64;  // 获取应用程序的键值（整数类型）

    // 检查是否已经存在该应用程序的网络标识，如果存在则警告并替换
    const auto iter = _netIdentify.find(appKey);
    if (iter != _netIdentify.end())
    {
        LOG_WARN("connector locator received multiple sockets for the same app.");
        _netIdentify.erase(appKey);  // 移除旧的网络标识
    }

    // 创建新的 NetworkIdentify 对象，包含 SocketKey 和 ObjectKey
    NetworkIdentify networkIdentify(socket, std::move(objKey));
    // 将应用程序键值和网络标识关联
    _netIdentify.insert(std::make_pair(appKey, networkIdentify));

    _lock.unlock();  // 解锁

    // 获取应用程序类型
    auto appType = GetTypeFromAppKey(appKey);
    // 输出调试信息，显示已连接的应用程序类型和 ID
    LOG_DEBUG("Connected to appType:" << GetAppName(appType) << " appId:" << GetIdFromAppKey(appKey) << " " << (NetworkIdentify*)&networkIdentify);

    // 如果连接的应用程序类型包含 APP_APPMGR（应用程序管理器）
    if ((appType & APP_APPMGR) != 0)
    {
        // 向应用程序管理器发送注册消息
        Proto::AppRegister proto;
        const auto pGlobal = Global::GetInstance();  // 获取全局对象
        proto.set_type(pGlobal->GetCurAppType());    // 设置当前应用程序类型
        proto.set_id(pGlobal->GetCurAppId());        // 设置当前应用程序 ID
        // 发送 MI_AppRegister 消息给 APP_APPMGR
        MessageSystemHelp::SendPacket(Proto::MsgId::MI_AppRegister, proto, APP_APPMGR);
    }
}

/**
 * @brief 获取指定应用程序类型的所有网络标识列表。
 * 
 * @param appType 应用程序类型
 * @return std::list<NetworkIdentify> 包含所有匹配的 NetworkIdentify 对象的列表
 */
std::list<NetworkIdentify> NetworkLocator::GetAppNetworks(const APP_TYPE appType)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全
    std::list<NetworkIdentify> rs;             // 定义结果列表

    auto iter = _netIdentify.begin();
    while (iter != _netIdentify.end()) {
        // 使用 std::find_if 查找匹配的应用程序类型
        iter = std::find_if(iter, _netIdentify.end(), [appType](auto pair)
            {
                auto appKey = pair.first;
                if (GetTypeFromAppKey(appKey) == appType)
                    return true;

                return false;
            });

        if (iter != _netIdentify.end())
        {
            rs.emplace_back(iter->second);  // 将匹配的 NetworkIdentify 添加到结果列表
            ++iter;  // 继续下一个迭代
        }
    }

    return rs;  // 返回匹配的网络标识列表
}

/**
 * @brief 移除指定应用程序键值对应的网络标识，并尝试重新建立连接。
 * 
 * @param appKey 应用程序键值（由应用程序类型和 ID 组成）
 */
void NetworkLocator::RemoveNetworkIdentify(uint64 appKey)
{
    _lock.lock();              // 加锁，手动管理锁的粒度
    _netIdentify.erase(appKey);  // 从网络标识映射中移除指定的应用程序键值
    _lock.unlock();            // 解锁

    // 输出调试信息，显示已移除的应用程序类型和 ID
    LOG_DEBUG("Removed appType:" << GetAppName(GetTypeFromAppKey(appKey)) << " appId:" << GetIdFromAppKey(appKey));

    // 尝试重新建立连接
    auto appType = GetTypeFromAppKey(appKey);
    auto appId = GetIdFromAppKey(appKey);
    const auto pYaml = ComponentHelp::GetYaml();             // 获取 YAML 配置组件
    const auto pCommonConfig = pYaml->GetIPEndPoint(appType, appId);  // 获取应用程序的 IP 和端口配置
    ObjectKey key{ ObjectKeyType::App, {appKey, ""} };       // 创建新的 ObjectKey 对象
    // 创建连接请求，重新连接到指定的应用程序
    MessageSystemHelp::CreateConnect(NetworkType::TcpConnector, key, pCommonConfig->Ip, pCommonConfig->Port);
}

/**
 * @brief 根据网络类型获取对应的网络连接器对象。
 * 
 * @param networkType 网络类型
 * @return INetwork* 指向对应网络连接器的指针，如果不存在则返回 nullptr
 */
INetwork* NetworkLocator::GetConnector(NetworkType networkType)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全
    auto iter = _connectors.find(networkType);  // 查找指定网络类型的连接器
    if (iter == _connectors.end())
        return nullptr;  // 未找到，返回空指针

    return iter->second;  // 返回找到的网络连接器指针
}

/**
 * @brief 根据应用程序类型和 ID 获取对应的网络标识。
 * 
 * @param appType 应用程序类型
 * @param appId 应用程序 ID
 * @return NetworkIdentify 返回对应的网络标识，如果未找到则返回默认构造的 NetworkIdentify 对象
 */
NetworkIdentify NetworkLocator::GetNetworkIdentify(const APP_TYPE appType, const int appId)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全

    const auto appKey = GetAppKey(appType, appId);  // 生成应用程序键值
    const auto iter = _netIdentify.find(appKey);    // 查找对应的网络标识
    if (iter == _netIdentify.end())
        return NetworkIdentify();  // 未找到，返回默认构造的对象

    return iter->second;  // 返回找到的网络标识
}

/**
 * @brief 添加网络监听器定位器，映射网络类型到具体的网络监听器对象。
 * 
 * @param pNetwork 指向网络监听器对象的指针
 * @param networkType 网络类型（如 TcpListen、HttpListen）
 */
void NetworkLocator::AddListenLocator(INetwork* pNetwork, NetworkType networkType)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全
    _listens[networkType] = pNetwork;          // 将网络类型与网络监听器对象关联
}

/**
 * @brief 根据网络类型获取对应的网络监听器对象。
 * 
 * @param networkType 网络类型
 * @return INetwork* 指向对应网络监听器的指针，如果不存在则返回 nullptr
 */
INetwork* NetworkLocator::GetListen(NetworkType networkType)
{
    std::lock_guard<std::mutex> guard(_lock);  // 加锁，保证线程安全
    auto iter = _listens.find(networkType);     // 查找指定网络类型的监听器
    if (iter == _listens.end())
        return nullptr;  // 未找到，返回空指针

    return iter->second;  // 返回找到的网络监听器指针
}

/**
 * @brief 处理应用程序注册的消息，当收到 MI_AppRegister 消息时调用。
 * 
 * @param pPacket 指向接收到的消息包的指针
 */
void NetworkLocator::HandleAppRegister(Packet* pPacket)
{
    // 解析消息包中的 Proto::AppRegister 协议
    const auto proto = pPacket->ParseToProto<Proto::AppRegister>();
    // 生成应用程序键值
    const uint64 appKey = GetAppKey(proto.type(), proto.id());
    // 创建新的 ObjectKey 对象
    ObjectKey objKey{ ObjectKeyType::App, ObjectKeyValue{ appKey, "" } };
    // 添加网络标识，将当前连接的 SocketKey 和应用程序的 ObjectKey 关联
    AddNetworkIdentify(pPacket->GetSocketKey(), objKey);
}
