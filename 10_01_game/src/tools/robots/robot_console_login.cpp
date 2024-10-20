#include "robot_console_login.h"

#include "libserver/common.h"
#include "libserver/thread_mgr.h"

#include <iostream>
#include "global_robots.h"
#include "robot_component_create.h"
#include "libserver/yaml.h"
#include "libserver/global.h"

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
    ThreadMgr::GetInstance()->GetEntitySystem()->AddComponent<RobotComponentCreate>(params[0],0,0);
    
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

    const auto pGlobal = Global::GetInstance();
    auto pYaml = pThreadMgr->GetEntitySystem()->GetComponent<Yaml>();
    const auto pConfig = pYaml->GetConfig(pGlobal->GetCurAppType());
    const auto pAppConfig = dynamic_cast<AppConfig*>(pConfig);

    // �߳�����
    auto threadCnt = pAppConfig->LogicThreadNum;
    threadCnt = threadCnt <= 0 ? 1 : threadCnt;

    // ÿ���߳���robot����
    int perThreadRobotCnt = static_cast<int>(std::ceil(count / threadCnt));
    perThreadRobotCnt = perThreadRobotCnt < 1 ? 1 : perThreadRobotCnt;

    for (int i = 0; i < threadCnt; i++)
    {
        const int min = i * perThreadRobotCnt;
        int max = min + perThreadRobotCnt;
        max = max > count ? count : max;
        pThreadMgr->CreateComponent<RobotComponentCreate>(LogicThread, false, params[0], min, max);

        if (max >= count)
            break;
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
