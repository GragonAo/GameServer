#include "robot_console_login.h"
#include "robot.h"

#include "libserver/common.h"
#include "libserver/thread_mgr.h"

#include <iostream>
#include "global_robots.h"

// 注册控制台命令处理程序
void RobotConsoleLogin::RegisterHandler()
{
    // 注册 -a 选项，绑定 HandleLogin 函数
    OnRegisterHandler("-a", BindFunP1(this, &RobotConsoleLogin::HandleLogin));
    
    // 注册 -ex 选项，绑定 HandleLoginEx 函数
    OnRegisterHandler("-ex", BindFunP1(this, &RobotConsoleLogin::HandleLoginEx));
    
    // 注册 -clean 选项，绑定 HandleLoginClean 函数
    OnRegisterHandler("-clean", BindFunP1(this, &RobotConsoleLogin::HandleLoginClean));
}

// 显示帮助信息
void RobotConsoleLogin::HandleHelp()
{
    std::cout << "\t-a account.\t\tlogin by account" << std::endl; // 使用账户登录
    std::cout << "\t-ex account count.\tbatch login, account is prefix, count as number" << std::endl; // 批量登录
    std::cout << "\t-clean.\t\tclean all logined account." << std::endl; // 清理所有登录的账户
}

// 处理普通登录命令
void RobotConsoleLogin::HandleLogin(std::vector<std::string>& params)
{
    // 检查参数数量是否为1
    if (!CheckParamCnt(params, 1))
        return;

    // 将机器人实例添加到实体系统中，并设置其账户名
    ThreadMgr::GetInstance()->GetEntitySystem()->AddComponent<Robot>(params[0]);
    
    // 更新全局机器人数量
    GlobalRobots::GetInstance()->SetRobotsCount(1);
}

// 处理增强的批量登录命令
void RobotConsoleLogin::HandleLoginEx(std::vector<std::string>& params) const
{
    // 检查参数数量是否为2
    if (!CheckParamCnt(params, 2))
        return;

    auto pThreadMgr = ThreadMgr::GetInstance();

    // 获取批量登录的账户数量
    const int count = std::atoi(params[1].c_str());
    if (count == 0)
    {
        std::cout << "input is error. see: -help" << std::endl; // 输入错误提示
        return;
    }

    // 创建多个机器人实例，每个实例的账户名由前缀和序号组成
    for (int i = 1; i <= count; i++)
    {
        std::string account = params[0] + std::to_string(i);
        pThreadMgr->CreateComponent<Robot>(account); // 添加机器人组件
    }

    // 更新全局机器人数量
    GlobalRobots::GetInstance()->SetRobotsCount(count);
}

// 处理清理登录状态命令
void RobotConsoleLogin::HandleLoginClean(std::vector<std::string>& params)
{
    std::cout << "close all." << std::endl; // 输出关闭所有的提示
    _threads.clear(); // 清空线程列表
    GlobalRobots::GetInstance()->CleanRobotsCount(); // 清理全局机器人数量
}
