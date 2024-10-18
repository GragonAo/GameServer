#pragma once

#include "libserver/state_template.h"
#include "libserver/robot_state_type.h"
#include "libplayer/player.h"
#include "robot_state.h"

class Robot : public Player, public StateTemplateMgr<RobotStateType, RobotState, Robot>, public IAwakeFromPoolSystem<std::string>
{
public:
    // 判断机器人是否为单例（此处设定为 false，表示可以有多个机器人实例）
    static bool IsSingle() { return false; }

    // 从对象池中唤醒时调用的初始化方法
    void Awake(std::string account) override;
    
    // 回收到对象池时调用，用于清理对象状态
    void BackToPool() override;
    
    // 更新方法，每帧调用，执行状态检查等逻辑
    void Update();

    // 处理网络断开时的操作
    void NetworkDisconnect();

protected:
    // 注册机器人状态机中的状态
    void RegisterState() override;

private:
    // 处理网络连接成功时的操作
    void HandleNetworkConnected(Robot* pRobot, Packet* pPacket);
    
    // 处理网络断开时的操作
    void HandleNetworkDisconnect(Robot* pRobot, Packet* pPacket);
    
    // 处理 HTTP 外部响应
    void HandleHttpOuterResponse(Robot* pRobot, Packet* pPacket);

    // 处理账号验证响应
    void HandleAccountCheckRs(Robot* pRobot, Packet* pPacket);
    
    // 处理玩家列表响应
    void HandlePlayerList(Robot* pRobot, Packet* pPacket);
    
    // 处理游戏 Token 响应
    void HandleGameToken(Robot* pRobot, Packet* pPacket);
    
    // 处理通过 Token 登录的响应
    void HandleLoginByTokenRs(Robot* pRobot, Packet* pPacket);
};
