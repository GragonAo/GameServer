#include "thread.h"
#include <iostream>

bool Thread::Start(){
    _isRun = true;
    _thread = std::thread([this](){
        while(_isRun) Update();
    });
    std::cout << "Thread Start Id:"<<_thread.get_id() << std::endl;
    return true;
}
void Thread::Stop(){
    if(_isRun) _isRun = false;
}
void Thread::Dispose(){
    Stop();
    if(_thread.joinable()) _thread.join();    //join会阻塞主线程直到线程关闭
}
void Thread::Update(){}