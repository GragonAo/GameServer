#pragma once
#include "libserver/component.h"
#include "libserver/system.h"

// 机器人组件创建类，继承自 Component 模板类和 IAwakeSystem 接口类
class RobotComponentCreate : public Component<RobotComponentCreate>, public IAwakeSystem<std::string, int, int>
{
public:
    // 唤醒函数，传入参数：账号（string），最小值（int），最大值（int）
    void Awake(std::string account, int min, int max) override;
    
    // 返回对象池时的清理函数
    void BackToPool() override;
};
