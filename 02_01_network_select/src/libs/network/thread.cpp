#include "thread.h"

// 启动线程的函数
bool Thread::Start() {
    // 标记线程为正在运行
    _isRun = true;
    
    // 创建并启动一个新线程，线程执行体为一个lambda表达式
    _thread = std::thread([this]() {
        // 只要 _isRun 为 true，就循环执行 Update() 函数
        while (_isRun) {
            Update();
        }
    });
    
    // 启动成功，返回 true
    return true;
}

// 停止线程的函数
void Thread::Stop() {
    // 如果线程正在运行，则将 _isRun 设置为 false，终止线程循环
    if (_isRun) {
        _isRun = false;
    }
}

// 释放线程资源的函数
void Thread::Dispose() {
    // 首先停止线程
    Stop();
    
    // 检查线程是否可连接（joinable），如果是，则调用 join() 函数
    if (_thread.joinable()) {
        _thread.join();  // 等待线程执行完毕
    }
}

// 线程循环中每次迭代执行的函数，具体实现需要在子类中定义
void Thread::Update() {
    // 子类应重写此方法以执行具体的任务
}