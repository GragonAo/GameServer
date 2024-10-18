#include "sync_component.h"
#include "log4_help.h"
#include "packet.h"
#include "component_help.h"

// 解析 Proto::AppInfoSync 并填充 AppInfo 对象
bool AppInfo::Parse(Proto::AppInfoSync proto)
{
    auto pYaml = ComponentHelp::GetYaml();  // 获取 YAML 配置组件
    this->AppId = proto.app_id();           // 从 proto 中获取 AppId
    this->AppType = static_cast<APP_TYPE>(proto.app_type());  // 转换并设置应用类型
    this->Online = proto.online();          // 获取在线人数

    // 根据 AppType 和 AppId 从 YAML 配置中获取 IP 和端口信息
    auto pConfig = pYaml->GetIPEndPoint(this->AppType, this->AppId);
    if (pConfig == nullptr)                // 如果找不到配置，返回 false
        return false;

    this->Ip = pConfig->Ip;                // 设置 IP 地址
    this->Port = pConfig->Port;            // 设置端口
    return true;
}

// 解析 Proto::AppInfoSync 并更新或添加到 _apps 容器中
void SyncComponent::Parse(Proto::AppInfoSync proto, SOCKET socket)
{
    const auto iter = _apps.find(proto.app_id());  // 查找是否已有该 AppId 的信息
    if (iter == _apps.end())  // 如果没有找到，添加新的 AppInfo
    {
        AppInfo syncAppInfo;
        if (syncAppInfo.Parse(proto))  // 解析 proto 并填充 syncAppInfo
        {
            syncAppInfo.Socket = socket;   // 设置 Socket 连接
            _apps[syncAppInfo.AppId] = syncAppInfo;  // 存储到 _apps 中
        }
    }
    else  // 如果已存在该 AppId，则更新信息
    {
        const int appId = proto.app_id();
        _apps[appId].Online = proto.online();  // 更新在线人数
        _apps[appId].Socket = socket;          // 更新 Socket
    }
}

// 处理应用程序信息同步的网络数据包
void SyncComponent::AppInfoSyncHandle(Packet* pPacket)
{
    const auto proto = pPacket->ParseToProto<Proto::AppInfoSync>();  // 将数据包解析为 Proto::AppInfoSync
    Parse(proto, pPacket->GetSocketKey().Socket);  // 调用 Parse 处理同步信息
}

// 根据应用类型获取一个应用程序的信息，返回成功与否
bool SyncComponent::GetOneApp(APP_TYPE appType, AppInfo* pInfo)
{
    if (_apps.empty())  // 如果没有应用程序信息，输出错误日志并返回 false
    {
        LOG_ERROR("GetApp failed. no more. appType:" << GetAppName(appType));
        return false;
    }

    // 查找第一个满足条件的应用程序
    auto iter = std::find_if(_apps.begin(), _apps.end(), [&appType](auto pair)
    {
        return (pair.second.AppType & appType) != 0;
    });

    if (iter == _apps.end())  // 如果找不到对应的应用程序类型，输出错误日志并返回 false
    {
        LOG_ERROR("GetApp failed. no more. appType:" << appType);
        return false;
    }

    // 找到在线人数最少的应用程序
    auto min = iter->second.Online;  // 记录在线人数最少的应用程序
    int appId = iter->first;
    for (; iter != _apps.end(); ++iter)  // 遍历所有应用程序
    {
        if (min == 0)  // 如果找到在线人数为 0 的应用程序，提前结束查找
            break;

        if ((iter->second.AppType & appType) == 0)  // 跳过类型不匹配的应用程序
            continue;

        if (iter->second.Online < min)  // 找到在线人数更少的应用程序，更新最小值
        {
            min = iter->second.Online;
            appId = iter->first;
        }
    }

    // 更新找到的应用程序的在线人数并返回该应用程序的信息
    _apps[appId].Online += 1;
    *pInfo = _apps[appId];
    return true;
}

// 输出当前所有同步的应用程序信息
void SyncComponent::CmdShow()
{
    LOG_DEBUG("------------------------------------");
    for (auto pair : _apps)  // 遍历所有应用程序
    {
        LOG_DEBUG("appId:" << std::setw(4) << pair.first <<
                  " type:" << GetAppName(pair.second.AppType) <<
                  " online:" << pair.second.Online);  // 输出应用程序 ID、类型和在线人数
    }
}
