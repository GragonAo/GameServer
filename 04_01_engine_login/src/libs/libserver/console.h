#pragma once

#include "disposable.h"
#include "thread_obj.h"
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#define ConsoleMaxBuffer 512

typedef std::function<void(std::string, std::string)> HandleConsole;

class ConsoleCmd : public IDisposable {
public:
  virtual void RegisterHandler() = 0;
  void Dispose() override;
  void Process(std::vector<std::string> params);

protected:
  void OnRegisterHandler(std::string key, HandleConsole handler);

private:
  std::map<std::string, HandleConsole> _handles;
};
class Console : public ThreadObject {
public:
  bool Init() override;
  void Dispose() override;
  void Update() override;
  void RegisterMsgFunction() override;

protected:
  template <class T> void Register(std::string cmd);

protected:
  std::map<std::string, std::shared_ptr<ConsoleCmd>> _handles;
  std::mutex _lock;
  std::thread _thread;
  std::queue<std::string> _commands;
};
