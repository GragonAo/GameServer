#include "global_robots.h"

// 获取当前机器人数量，线程安全
size_t GlobalRobots::GetRobotsCount()
{
    // 使用 std::lock_guard 对互斥锁 _mtx 进行加锁，保证在读取 _robotsCnt 期间其他线程不会修改它
    std::lock_guard<std::mutex> guard(_mtx);
    return _robotsCnt;  // 返回机器人数量
}

// 设置机器人数量，线程安全
void GlobalRobots::SetRobotsCount(const size_t count)
{
    // 使用 std::lock_guard 对互斥锁 _mtx 进行加锁，确保在修改 _robotsCnt 期间其他线程不会读取或写入该变量
    std::lock_guard<std::mutex> guard(_mtx);
    _robotsCnt = count;  // 设置机器人数量
}

// 清除机器人数量，将其重置为 0，线程安全
void GlobalRobots::CleanRobotsCount()
{
    // 使用 std::lock_guard 对互斥锁 _mtx 进行加锁，确保清零操作不会受到其他线程的干扰
    std::lock_guard<std::mutex> guard(_mtx);
    _robotsCnt = 0;  // 将机器人数量重置为 0
}
