#include "robot_state_login.h"
#include "robot.h"
#include "libserver/message_system_help.h"
#include "libserver/component_help.h"
#include "robot_component_login.h"

// 当机器人进入登录连接状态时调用
void RobotStateLoginConnecting::OnEnterState()
{
    // 断开已有的网络连接，确保从头开始新的连接
    _pParentObj->NetworkDisconnect();

    // 获取机器人登录组件，包含登录IP和端口
    const auto loginObj = _pParentObj->GetComponent<RobotComponentLogin>();

    // 创建TCP连接，连接到登录服务器
    MessageSystemHelp::CreateConnect(NetworkType::TcpConnector, 
                                     _pParentObj->GetObjectKey(), 
                                     loginObj->GetLoginIp(), 
                                     loginObj->GetLoginPort());
}

// 每次更新时调用，用来检查是否连接成功
RobotStateType RobotStateLoginConnecting::OnUpdate()
{
    // 获取当前机器人的Socket信息
    const auto socketKey = _pParentObj->GetSocketKey();

    // 如果Socket合法且网络类型为TcpConnector，则表示连接成功，切换到已连接状态
    if (socketKey.Socket != INVALID_SOCKET && socketKey.NetType == NetworkType::TcpConnector)
    {
        return RobotStateType::Login_Connected;  // 返回已连接状态
    }

    // 如果尚未连接成功，则保持当前状态
    return GetState();
}

// 当机器人进入登录成功连接状态时调用
void RobotStateLoginConnected::OnEnterState()
{
    // 创建账号验证请求的消息对象
    Proto::AccountCheck accountCheck;

    // 设置账号和密码（此处的密码为MD5加密后的值）
    accountCheck.set_account(_pParentObj->GetAccount().c_str());
    accountCheck.set_password("e10adc3949ba59abbe56e057f20f883e"); // "123456" 的 MD5 加密值

    // 向登录服务器发送账号验证请求
    MessageSystemHelp::SendPacket(Proto::MsgId::C2L_AccountCheck, _pParentObj, accountCheck );

#ifdef LOG_TRACE_COMPONENT_OPEN
    // 如果日志追踪功能开启，记录登录过程的追踪信息
    std::stringstream traceMsg;
    traceMsg << "send check account.";  // 记录发送账号验证信息
    traceMsg << " account:" << _pParentObj->GetAccount();
    traceMsg << " socket:" << _pParentObj->GetSocketKey().Socket;

    // 记录追踪信息到日志系统
    ComponentHelp::GetTraceComponent()->TraceAccount(_pParentObj->GetAccount(), _pParentObj->GetSocketKey().Socket);
    ComponentHelp::GetTraceComponent()->Trace(TraceType::Player, _pParentObj->GetSocketKey().Socket, traceMsg.str());
#endif
}

// 每次更新时调用，用来检查当前状态
RobotStateType RobotStateLoginConnected::OnUpdate()
{
    // 返回当前状态，保持为已连接状态
    return GetState();
}

// 当机器人已登录的状态更新时调用
RobotStateType RobotStateLoginLogined::OnUpdate()
{
    // 返回当前状态，保持为已登录状态
    return GetState();
}
