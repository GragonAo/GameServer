#pragma once

#include <thread>  // C++ 标准库的线程类
#include <list>    // C++ 标准库的列表容器

#include "sn_object.h"        // SN 对象的基类，提供唯一标识符（SN）功能
#include "system_manager.h"   // 系统管理器基类，管理系统组件和依赖
#include "thread_type.h"      // 线程类型的枚举定义

// 定义线程状态的枚举类型，用于表示线程的生命周期
enum class ThreadState
{
    Init,     // 初始化状态，线程尚未启动
    Run,      // 运行状态，线程正在执行
    Stop,     // 停止状态，线程已请求停止
    Destroy,  // 销毁状态，线程已销毁
};

// Thread 类继承自 SystemManager 和 SnObject，用于表示和管理单个线程
class Thread : public SystemManager, public SnObject
{
public:
    // 构造函数，接受线程类型作为参数
    Thread(ThreadType threadType);

    // 析构函数，用于清理资源
    ~Thread();

    // 启动线程，进入运行状态
    void Start();

    // 销毁线程，进入销毁状态
    void DestroyThread();

    // 重写 Dispose 方法，释放资源
    void Dispose() override;

    // 判断线程是否处于停止状态
    bool IsStop() const;

    // 判断线程是否已经被销毁
    bool IsDestroy() const;

private:
    // 线程类型，例如主线程、逻辑线程等
    ThreadType _threadType;

    // 当前线程的状态，使用 ThreadState 枚举
    ThreadState _state;

    // C++ 标准库中的线程对象，实际管理线程的创建与运行
    std::thread _thread;
};
