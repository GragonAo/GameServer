#pragma once
#include "libserver/console.h"

// RobotConsoleHttp 类继承自 ConsoleCmd，处理与 HTTP 请求相关的控制台命令
class RobotConsoleHttp : public ConsoleCmd
{
public:
    // 注册控制台命令的处理程序
    void RegisterHandler() override;

    // 显示帮助信息
    void HandleHelp() override;

private:    
    // 处理 HTTP 请求的命令
    void HandleRequest(std::vector<std::string>& params);
};
