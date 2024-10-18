#pragma once
#include "libserver/console.h"
#include <list>

// 机器人登录控制台命令类，继承自 ConsoleCmd 基类
// 该类实现了一些控制台命令，用于处理机器人登录的操作
class RobotConsoleLogin :public ConsoleCmd
{
public:
    // 注册处理程序函数
    // 该函数用来注册不同命令的处理器，比如登录、批量登录等
    void RegisterHandler() override;
    
    // 帮助命令处理程序函数
    // 当用户输入帮助命令时，展示可用命令的说明
    void HandleHelp() override;

private:
    // 处理单个登录命令的函数
    // 参数：params - 包含登录命令和参数的字符串向量
    void HandleLogin(std::vector<std::string>& params);
    
    // 处理批量登录命令的函数
    // 该函数处理带有账户前缀和登录数量的批量登录请求
    // 参数：params - 包含批量登录命令和参数的字符串向量
    void HandleLoginEx(std::vector<std::string>& params) const;
    
    // 处理清除所有登录的机器人线程的命令
    // 参数：params - 包含清除命令的字符串向量
    void HandleLoginClean(std::vector<std::string>& params);

private:
    // 用于存储机器人线程 ID 的列表
    std::list<uint64> _threads;
};
