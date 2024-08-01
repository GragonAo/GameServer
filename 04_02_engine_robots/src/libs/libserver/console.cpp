#include "console.h"
#include "thread_obj.h"
#include "util_string.h"
#include <iostream>
#include <thread>
#include <vector>

// 清空命令处理器的所有处理函数
void ConsoleCmd::Dispose() { 
    _handles.clear(); 
}

// 处理输入的命令参数
void ConsoleCmd::Process(std::vector<std::string> params) {
    if (params.size() <= 1)
        return;
    
    const std::string key = params[1];
    const auto iter = _handles.find(key);
    
    if (iter == _handles.end()) {
        // 如果命令关键字没有找到，打印错误信息
        std::cout << "input error. can't find cmd:" << key.c_str() << std::endl;
        return;
    }

    // 根据参数数量调用相应的处理函数
    switch (params.size()) {
        case 2:
            iter->second("", "");
            break;
        case 3:
            iter->second(params[2], "");
            break;
        case 4:
            iter->second(params[2], params[3]);
            break;
        default:
            std::cout << "input error. -help for help." << std::endl;
            break;
    }
}

// 注册一个新的命令处理函数
void ConsoleCmd::OnRegisterHandler(std::string key, HandleConsole handler) {
    _handles[key] = handler;
}

// 初始化控制台，启动一个新的线程来监听输入
bool Console::Init() {
    _thread = std::thread([this]() {
        char _buffer[ConsoleMaxBuffer];
        do {
            std::cin.getline(_buffer, ConsoleMaxBuffer);
            _lock.lock();
            _commands.push(std::string(_buffer));
            _lock.unlock();
        } while (true);
    });
    return true;
}

// 清理控制台，释放资源
void Console::Dispose() {
    for (auto &pair : _handles) {
        pair.second->Dispose();
    }
    _handles.clear();
    ThreadObject::Dispose();
}

// 更新控制台，处理输入命令
void Console::Update() {
    _lock.lock();
    if (_commands.size() <= 0) {
        _lock.unlock();
        return;
    }
    
    const std::string cmd = _commands.front();
    _commands.pop();
    _lock.unlock();

    std::vector<std::string> params;
    strutil::split(cmd, ' ', params);

    if (params.size() <= 0)
        return;

    const std::string key = params[0];
    const auto iter = _handles.find(key);

    if (iter == _handles.end()) {
        // 如果命令关键字没有找到，打印错误信息
        std::cout << "input error. comd:" << cmd.c_str() << std::endl;
        return;
    }

    // 调用对应的命令处理函数
    iter->second->Process(params);
}

// 注册消息处理函数（目前未实现）
void Console::RegisterMsgFunction() {}
