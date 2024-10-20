#include "app_sync_component.h"
#include "libserver/message_system_help.h"
#include "libserver/message_system.h"
// AppSyncComponent类实现：处理应用信息的同步、命令、HTTP请求等功能

void AppSyncComponent::Awake()
{
    // 添加一个定时器，每隔2秒执行一次SyncGameInfoToLogin函数
    AddTimer(0, 2, false, 0, BindFunP0(this, &AppSyncComponent::SyncGameInfoToLogin));

    // 初始化JSON流写入器，用于在HTTP请求时生成JSON格式的返回值
    Json::StreamWriterBuilder jsonBuilder;
    _jsonWriter = jsonBuilder.newStreamWriter();

    // 注册消息回调处理
    auto pMsgSystem = GetSystemManager()->GetMessageSystem();

    // 注册HTTP登录请求的回调处理函数，收到此消息时调用HandleHttpRequestLogin函数
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_HttpRequestLogin, BindFunP1(this, &AppSyncComponent::HandleHttpRequestLogin));

    // 注册应用信息同步消息的回调处理函数，收到此消息时调用HandleAppInfoSync函数
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_AppInfoSync, BindFunP1(this, &AppSyncComponent::HandleAppInfoSync));

    // 注册命令消息的回调处理函数，收到此消息时调用HandleCmdApp函数
    pMsgSystem->RegisterFunction(this,Proto::MsgId::MI_CmdApp, BindFunP1(this, &AppSyncComponent::HandleCmdApp));
}

void AppSyncComponent::BackToPool()
{
    // 销毁JSON写入器，释放资源
    delete _jsonWriter;

    // 清空保存的应用信息
    _apps.clear();
}

// 处理应用命令消息
void AppSyncComponent::HandleCmdApp(Packet* pPacket)
{
    // 解析接收到的Proto协议中的CmdApp消息
    auto cmdProto = pPacket->ParseToProto<Proto::CmdApp>();

    // 判断命令类型
    auto cmdType = cmdProto.cmd_type();
    if (cmdType == Proto::CmdApp_CmdType_Info)
    {
        // 如果命令类型为Info，则调用CmdShow显示当前信息
        CmdShow();
    }
}

// 处理HTTP登录请求的消息
void AppSyncComponent::HandleHttpRequestLogin(Packet* pPacket)
{
    // 构建JSON对象，用于返回登录请求的结果
    Json::Value responseObj;

    // 获取一个登录服务器的信息
    AppInfo info;
    if (!GetOneApp(APP_LOGIN, &info))
    {
        // 如果没有可用的登录服务器，则返回超时的错误码
        responseObj["returncode"] = (int)Proto::LoginHttpReturnCode::LHRC_TIMEOUT;
        responseObj["ip"] = "";
        responseObj["port"] = 0;
    }
    else
    {
        // 如果获取到登录服务器，返回登录服务器的IP和端口
        responseObj["returncode"] = (int)Proto::LoginHttpReturnCode::LHRC_OK;
        responseObj["ip"] = info.Ip;
        responseObj["port"] = info.Port;
    }

    // 将JSON对象转换为字符串流
    std::stringstream jsonStream;
    _jsonWriter->write(responseObj, &jsonStream);

    // 发送HTTP响应消息
    MessageSystemHelp::SendHttpResponse(pPacket, jsonStream.str().c_str(), jsonStream.str().length());
}

// 处理应用信息同步消息
void AppSyncComponent::HandleAppInfoSync(Packet* pPacket)
{
    // 将接收到的应用信息同步处理
    AppInfoSyncHandle(pPacket);
}

// 定时同步游戏服务器的信息到登录服务器
void AppSyncComponent::SyncGameInfoToLogin()
{
    // 创建用于同步的Proto消息
    Proto::AppInfoListSync proto;

    // 遍历所有应用，筛选出游戏服务器的信息
    for (auto pair : _apps)
    {
        auto info = pair.second;

        // 仅同步游戏服务器信息
        if ((info.AppType & APP_GAME) == 0)
            continue;

        // 将游戏服务器的信息加入到同步消息中
        auto pProto = proto.add_apps();
        pProto->set_app_id(info.AppId);
        pProto->set_app_type(info.AppType);
        pProto->set_online(info.Online);
    }

    // 发送同步消息到所有登录服务器
    MessageSystemHelp::SendPacketToAllApp(Proto::MsgId::MI_AppInfoListSync, proto, APP_LOGIN);
}
