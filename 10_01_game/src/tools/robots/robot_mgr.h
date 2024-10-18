#pragma once

#include "libserver/robot_state_type.h"
#include "libserver/entity.h"

class RobotMgr : public Entity<RobotMgr>, public IAwakeSystem<>
{
public:
    // 初始化方法，在系统启动时调用
    void Awake() override;
    
    // 对象池回收时调用，清空对象的状态
    void BackToPool() override;

    // 显示机器人管理相关信息
    void ShowInfo();

private:
    // 处理来自机器人状态变更的消息
    void HandleRobotState(Packet* pPacket);

    // 通知服务器机器人状态发生变化
    void NotifyServer(RobotStateType iType);

private:
    // 记录程序启动的时间点，用于统计
    std::chrono::system_clock::time_point _start;
    
    // 标志位，用于跟踪是否有状态变更
    bool _isChange{ false };

    // 机器人状态映射表，key 是账户名，value 是机器人状态
    std::map<std::string, RobotStateType> _robots;
};
