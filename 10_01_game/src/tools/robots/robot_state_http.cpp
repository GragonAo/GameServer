#include "robot_state_http.h"
#include "robot.h"
#include "libserver/message_system_help.h"
#include "libserver/component_help.h"

// 当进入 HTTP 连接状态时触发的逻辑
void RobotStateHttpConnecting::OnEnterState()
{
    
    // 获取 YAML 配置文件
    const auto pYaml = ComponentHelp::GetYaml();

    // 从配置文件中获取 APP 管理器的配置信息，主要是 IP 和 HTTP 端口
    const auto pCommonConfig = dynamic_cast<CommonConfig*>(pYaml->GetConfig(APP_APPMGR));

    // 创建一个 ObjectKey，用于标识账号的唯一性
    ObjectKey key{ ObjectKeyType::Account, {0, _pParentObj->GetAccount()} };

    // 创建一个 HTTP 连接，指定连接类型为 HttpConnector，使用从配置中获取的 IP 和端口
    MessageSystemHelp::CreateConnect(NetworkType::HttpConnector, key, pCommonConfig->Ip, pCommonConfig->HttpPort);
}

// 更新 HTTP 连接状态的逻辑
// 如果连接已建立（即 socket 有效且连接类型为 HttpConnector），则切换到 "Http_Connected" 状态
RobotStateType RobotStateHttpConnecting::OnUpdate()
{
    const auto socketKey = _pParentObj->GetSocketKey();
    if (socketKey.Socket != INVALID_SOCKET && socketKey.NetType == NetworkType::HttpConnector)
    {
        return RobotStateType::Http_Connected;  // HTTP 已连接，状态切换到 Http_Connected
    }

    return GetState();  // 否则保持当前状态
}

// 当进入 HTTP 已连接状态时触发的逻辑
void RobotStateHttpConnected::OnEnterState()
{
#ifdef LOG_TRACE_COMPONENT_OPEN
    // 如果开启了日志跟踪，则记录日志
    std::stringstream traceMsg;
    traceMsg << "http connected.";  // 记录 HTTP 连接成功的日志信息
    traceMsg << " account:" << _pParentObj->GetAccount();  // 记录账号信息
    traceMsg << " socket:" << _pParentObj->GetSocketKey().Socket;  // 记录 socket 信息

    // 使用组件帮助类记录追踪信息
    ComponentHelp::GetTraceComponent()->TraceAccount(_pParentObj->GetAccount(), _pParentObj->GetSocketKey().Socket);
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Player, _pParentObj->GetSocketKey().Socket, traceMsg.str());
#endif

    // 获取 YAML 配置文件中的 APP 管理器配置信息
    const auto pYaml = ComponentHelp::GetYaml();
    const auto pCommonConfig = dynamic_cast<CommonConfig*>(pYaml->GetConfig(APP_APPMGR));

    // 创建 ObjectKey，用于标识账号的唯一性
    ObjectKey key{ ObjectKeyType::Account, {0, _pParentObj->GetAccount()} };

    // 发送 HTTP 请求，访问 "/login" 接口，执行登录操作
    MessageSystemHelp::SendHttpRequest(_pParentObj, pCommonConfig->Ip, pCommonConfig->HttpPort, "GET","/login", nullptr);
}

// 更新 HTTP 已连接状态的逻辑
// 目前仅保持当前状态，后续可根据需要扩展
RobotStateType RobotStateHttpConnected::OnUpdate()
{
    return GetState();  // 保持当前状态
}
