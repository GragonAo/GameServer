#include "robot_component_create.h"
#include "robot_collection.h"

#include "libserver/log4_help.h"
#include "libserver/system_manager.h"

#include <thread>

// 唤醒函数，负责根据传入的参数创建一批机器人
void RobotComponentCreate::Awake(std::string account, int min, int max)
{
    // 根据输入的 min 和 max 参数创建机器人，范围为 [min, max)
    // 打印日志信息，显示创建的账号信息和线程ID（注释掉的部分可以根据需要恢复使用）
    // LOG_DEBUG("create robot. account:" << account.c_str() << " min:" << min << " max:" << max << " thread id:" << std::this_thread::get_id());

    // 获取系统管理器中的机器人集合组件，用于管理和创建机器人
    auto pCollection = GetSystemManager()->GetEntitySystem()->GetComponent<RobotCollection>();
    
    // 当 min 和 max 都为 0 时，表示创建一个机器人
    if (min == 0 && max == 0)
    {
        pCollection->CreateRobot(account);  // 直接创建单个机器人，使用传入的 account 作为账号
    }
    else
    {
        // 当 min 和 max 不为 0 时，创建一批机器人，账号在 account 基础上添加序号后缀
        for (int i = min; i < max; i++)
        {
            pCollection->CreateRobot(account + std::to_string(i));  // 将 i 作为后缀拼接到账号字符串上
        }
    }
}

// 回收函数，当对象返回对象池时调用
void RobotComponentCreate::BackToPool()
{
    // 此函数目前没有实现，通常用于重置对象的状态
}
