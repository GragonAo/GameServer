#include "console.h"
#include "console_cmd_pool.h"
#include "thread_obj.h"
#include "util_string.h"
#include <iostream>
#include <thread>
#include <vector>
// 清空命令处理器的所有处理函数
void ConsoleCmd::Dispose() { _handles.clear(); }

bool ConsoleCmd::CheckParamCnt(std::vector<std::string> &params,
                               const size_t count) {
  if (params.size() == count)
    return true;
  std::cout << "input param size is error.see -help" << std::endl;
  return false;
}

// 处理输入的命令参数
void ConsoleCmd::Process(std::vector<std::string> &params) {
  if (params.size() <= 1)
    return;

  const std::string key = params[1];
  const auto iter = _handles.find(key);

  if (iter == _handles.end()) {
    // 如果命令关键字没有找到，打印错误信息
    std::cout << "input error. can't find cmd:" << key.c_str() << std::endl;
    return;
  }
  params.erase(params.begin(), params.begin() + 2);
  iter->second(params);
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

      std::string cmdMsg = _buffer;
      auto iter = cmdMsg.find("exit");
      if (iter != std::string::npos) {
        _isRun = false;
        Global::GetInstance()->IsStop = true;
      }
    } while (_isRun);
  });
  Register<ConsoleCmdPool>("pool");
  return true;
}

// 清理控制台，释放资源
void Console::Dispose() {
  for (auto &pair : _handles) {
    pair.second->Dispose();
  }
  _handles.clear();

  if (_isRun)
    _thread.detach();
  else
    _thread.join();

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
