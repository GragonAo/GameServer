#include "thread.h"          // 包含线程类的头文件
#include "global.h"          // 包含全局状态类，用于管理全局停止标志
#include "entity_system.h"    // 实体系统，用于组件更新
#include "log4.h"            // 日志系统，处理线程的日志记录
#include "component_help.h"   // 组件帮助类，用于初始化线程中的组件

#include <iterator>           // 标准库迭代器支持

// 构造函数，初始化线程状态和类型
Thread::Thread(ThreadType threadType)
{
    _state = ThreadState::Init;  // 初始化状态
    _threadType = threadType;    // 保存线程类型
}

// 析构函数，在线程对象销毁时执行
Thread::~Thread()
{
    // std::cout << "close thread [3/3] delete. " << std::endl;
    // 析构函数中没有特别的清理工作，资源在其他地方被销毁
}

// 启动线程
void Thread::Start()
{
    // 使用 lambda 函数在线程中运行任务
    _thread = std::thread([this]()
    {
        // 初始化线程中的组件，调用帮助类初始化函数
        InitComponent(_threadType);
        _state = ThreadState::Run;  // 设置线程状态为运行

        // 获取全局状态对象的指针
        auto pGlobal = Global::GetInstance();
        
        // 线程主循环，当全局停止标志没有被设置时不断更新
        while (!pGlobal->IsStop)
        {
            Update();  // 更新线程中的组件
            std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 让线程休眠 1 毫秒，避免 CPU 占用过高
        }

        // 线程执行完成后的清理工作
        Dispose();                  // 调用清理函数
        log4cplus::threadCleanup();  // 清理 log4cplus 线程本地资源
        _state = ThreadState::Stop;  // 设置线程状态为停止
    });
}

// 检查线程是否已停止
bool Thread::IsStop() const
{
    return _state == ThreadState::Stop;  // 返回线程是否处于停止状态
}

// 检查线程是否已销毁
bool Thread::IsDestroy() const
{
    return _state == ThreadState::Destroy;  // 返回线程是否处于销毁状态
}

// 销毁线程，确保线程正确退出
void Thread::DestroyThread()
{
    // 如果线程已经销毁，则无需再执行销毁操作
    if (_state == ThreadState::Destroy)
        return;

    // 如果线程仍然在运行，并且可以 join（等待线程结束）
    if (_thread.joinable())
    {
        _thread.join();  // 阻塞等待线程结束
        _state = ThreadState::Destroy;  // 设置线程状态为销毁
    }
}

// 清理资源，释放系统管理器中的资源
void Thread::Dispose()
{
    // 如果线程已经销毁，无需重复清理
    if (_state == ThreadState::Destroy)
        return;

    SystemManager::Dispose();  // 调用基类 SystemManager 的 Dispose 方法进行清理
}
