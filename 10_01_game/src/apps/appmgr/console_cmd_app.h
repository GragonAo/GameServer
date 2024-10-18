#pragma once  // 头文件保护，防止重复包含

#include "libserver/console.h"  // 引入控制台命令相关的库

// ConsoleCmdApp 类，继承自 ConsoleCmd，用于处理与应用相关的控制台命令
class ConsoleCmdApp : public ConsoleCmd
{
public:
    // 注册命令处理器
    void RegisterHandler() override;

    // 显示帮助信息
    void HandleHelp() override;

private:
    // 处理应用信息的命令
    void HandleAppInfo(std::vector<std::string>& params);
};
