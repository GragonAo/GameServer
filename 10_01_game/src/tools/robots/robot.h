#pragma once

// 包含必要的头文件
#include "libserver/state_template.h"  // 状态模板的基类
#include "libserver/robot_state_type.h"  // 机器人状态类型定义

#include "libplayer/player.h"  // 玩家类的定义

#include "robot_state.h"  // 机器人状态的定义

// 定义 Robot 类，继承自 Player、StateTemplateMgr，并实现 IAwakeFromPoolSystem 接口
class Robot : public Player, 
              public StateTemplateMgr<RobotStateType, RobotState, Robot>,  // 状态管理模板
              virtual public IAwakeFromPoolSystem<std::string>  // 从对象池中唤醒的系统接口
{
public:
    // 重写从对象池唤醒的方法
    void Awake(std::string account) override;

    // 重写返回对象池的方法
    void BackToPool() override;

    // 更新方法，进行每帧更新逻辑
    void Update();

    // 网络断开连接处理方法
    void NetworkDisconnect();

    // 设置套接字键
    void SetSocketKey(SocketKey key);

protected:
    // 注册状态，重写父类的注册方法
    void RegisterState() override;
};
