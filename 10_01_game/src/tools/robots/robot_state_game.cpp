#include "robot_state_game.h"
#include "robot.h"
#include "libserver/message_system_help.h"
#include "robot_component_gametoken.h"

// 游戏连接状态类的实现
void RobotStateGameConnecting::OnEnterState()
{
    // 断开当前网络连接
    _pParentObj->NetworkDisconnect();

    // 获取游戏令牌组件
    auto tokenObj = _pParentObj->GetComponent<RobotComponentGameToken>();

    // 创建一个新的 TCP 连接
    MessageSystemHelp::CreateConnect(NetworkType::TcpConnector, _pParentObj->GetObjectKey(), tokenObj->GetGameIp(), tokenObj->GetGamePort());
}

// 更新游戏连接状态的方法
RobotStateType RobotStateGameConnecting::OnUpdate()
{
    // 获取当前机器人的 socket 关键信息
    auto socketKey = _pParentObj->GetSocketKey();

    // 如果 socket 有效并且连接类型是 TcpConnector，则表示已经成功连接
    if (socketKey.Socket != INVALID_SOCKET && socketKey.NetType == NetworkType::TcpConnector)
    {
        return RobotStateType::Game_Connected; // 返回游戏已连接状态
    }

    return GetState(); // 返回当前状态
}

// 游戏已连接状态类的实现
void RobotStateGameConnected::OnEnterState()
{
    // 获取游戏令牌组件
    auto tokenObj = _pParentObj->GetComponent<RobotComponentGameToken>();

    // 创建登录请求消息
    Proto::LoginByToken protoLogin;
    protoLogin.set_account(_pParentObj->GetAccount().c_str()); // 设置账户名
    protoLogin.set_token(tokenObj->GetToken().c_str()); // 设置令牌

    // 发送登录请求包
    MessageSystemHelp::SendPacket(Proto::MsgId::C2G_LoginByToken, _pParentObj, protoLogin);
}
