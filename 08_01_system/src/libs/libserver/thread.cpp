#include "thread.h"
#include "global.h"
#include "system_manager.h"
#include "thread_type.h"
#include <chrono>
#include <log4cplus/config.hxx>

Thread::Thread(ThreadType threadType) {
  _threadType = threadType;
  _state = ThreadState::Init;
}

Thread::~Thread() {
  std::cout << "close thread [3/3] delete. type: "
            << GetThreadTypeName(_threadType) << std::endl;
}

void Thread::Start() {
  _thread = std::thread([this]() {
    InitComponent(_threadType);
    _state = ThreadState::Run;
    while (!Global::GetInstance()->IsStop) {
      Update();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    const auto theadId = _thread.get_id();
    std::cout << "close thread [1/3] stop. thread id: " << theadId << std::endl;

    Dispose();
    log4cplus::threadCleanup();
    _state = ThreadState::Stop;
  });
}

bool Thread::IsStop() const { return _state == ThreadState::Stop; }

bool Thread::IsDestroy() const { return _state == ThreadState::Destroy; }

void Thread::DestroyThread() {

  if (_state == ThreadState::Destroy)
    return;

  if (_thread.joinable()) {
    const auto theadId = _thread.get_id();
    _thread.join();
    std::cout << "close thread [2/3] join. thread id: " << theadId << std::endl;
    _state = ThreadState::Destroy;
  }
}

void Thread::Dispose() {
  if (_state == ThreadState::Destroy)
    return;

  SystemManager::Dispose();
}