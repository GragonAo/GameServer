#include "robot_state.h"
#include "libserver/packet.h"
#include "robot.h"
#include "libserver/thread_mgr.h"
#include "libserver/message_system.h"
#include "libserver/component_help.h"

// 主要逻辑：检查是否断开连接，返回状态
RobotStateType RobotState::Update()
{
    const auto state = GetState();
    
    // 如果机器人状态大于 Login_Connecting 且不等于 Game_Connecting，检查网络连接
    if (state > RobotStateType::Login_Connecting && state != RobotStateType::Game_Connecting)
    {
        const auto socketKey = _pParentObj->GetSocketKey();
        
        // 如果 socket 无效，返回 Login_Connecting 状态
        if (socketKey.Socket == INVALID_SOCKET)
        {
            return RobotStateType::Login_Connecting;
        }
    }

    // 调用自定义的 OnUpdate()，返回状态
    return OnUpdate();
}

void RobotState::EnterState()
{
    // 当进入一个新的状态时，通知 RobotMgr
    Proto::RobotSyncState protoState;
    
    // 添加机器人状态信息
    auto pState = protoState.add_states();
    pState->set_account(_pParentObj->GetAccount());  // 设置机器人账号
    pState->set_state((int)GetState());  // 设置当前状态

#ifdef LOG_TRACE_COMPONENT_OPEN
    if (_pParentObj->GetSocketKey().Socket != INVALID_SOCKET) {
        std::stringstream os;
        os << "enter state:" << GetRobotStateTypeShortName(GetState());
        ComponentHelp::GetTraceComponent()->Trace(TraceType::Player, _pParentObj->GetSocketKey().Socket, os.str());
    }
#endif

    // 发送状态同步消息到消息系统（仅发送到管理线程）
    auto pPacket = MessageSystemHelp::CreatePacket(Proto::MsgId::MI_RobotSyncState, nullptr);
    pPacket->SerializeToBuffer(protoState);
    ThreadMgr::GetInstance()->GetMessageSystem()->AddPacketToList(pPacket);
    
    // 增加引用计数以防止过早释放
    pPacket->OpenRef();

    // 调用自定义的状态进入逻辑
    OnEnterState();
}

void RobotState::LeaveState()
{
    // 调用自定义的状态离开逻辑
    OnLeaveState();
}
