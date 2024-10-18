#pragma once  // 防止头文件重复包含
#include "libserver/singleton.h"  // 包含单例模板类
#include <mutex>  // 用于线程锁

// GlobalRobots 类继承自 Singleton 模板类，实现了一个全局机器人数量的管理器
class GlobalRobots : public Singleton<GlobalRobots>
{
public:
    // 获取当前机器人的数量，线程安全
    size_t GetRobotsCount();

    // 设置机器人数量，线程安全
    void SetRobotsCount(size_t count);

    // 清除机器人数量，将其重置为 0，线程安全
    void CleanRobotsCount();

private:
    std::mutex _mtx;  // 互斥锁，保证线程安全
    size_t _robotsCnt{ 0 };  // 用于存储当前机器人的数量，初始值为 0
};
