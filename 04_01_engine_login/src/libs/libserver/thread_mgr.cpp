#include "thread_mgr.h"
#include "common.h"
#include "network_listen.h"
#include <mutex>
// 构造函数，初始化ThreadMgr
ThreadMgr::ThreadMgr() {}

// 启动所有线程
void ThreadMgr::StartAllThread() {
    auto iter = _threads.begin();
    while(iter != _threads.end()){
        iter->second->Start();
        ++iter;
    }
}

// 检查是否是游戏循环线程
bool ThreadMgr::IsGameLoop() {
    for(auto iter = _threads.begin();iter!=_threads.end();iter++){
        if(iter->second->IsRun()){
            return true;
        }
    }
    return false;
}

// 创建新线程
void ThreadMgr::NewThread() {
    std::lock_guard<std::mutex> guard(_thread_lock);
    auto pThread = new Thread;
    _threads.insert(std::make_pair(pThread->GetSN(), pThread));
}

// 将对象添加到线程中
bool ThreadMgr::AddObjToThread(ThreadObject *obj) {
    std::lock_guard<std::mutex> guard(_thread_lock);
    auto iter = _threads.begin();
    if(_lastThreadSn > 0){
        iter = _threads.find(_lastThreadSn);
    }
    if(iter == _threads.end()){
        std::cout << "AddThreadObj Failed. no thread."<<std::endl;
        return false;
    }
    do{
        ++iter;
        if(iter == _threads.end()){
            iter = _threads.begin();
        }
    }while(!(iter->second->IsRun()));
    auto pThread = iter->second;
    pThread->AddObject(obj);
    _lastThreadSn = pThread->GetSN();
    return true;
}

// 根据应用类型将网络对象添加到线程中
void ThreadMgr::AddNetworkToThread(APP_TYPE appType, Network *pNetwork) {
     if(!AddObjToThread(pNetwork))return;
     std::lock_guard<std::mutex>guard(_thread_lock);
     _networkLocator[appType] = pNetwork;
}

// 分发数据包
void ThreadMgr::DispatchPacket(Packet *pPacket) {
    AddPacketToList(pPacket);
    std::lock_guard<std::mutex> guard(_thread_lock);
    for(auto iter = _threads.begin();iter != _threads.end();iter++){
        Thread* pThread =  iter->second;
        pThread->AddPacketToList(pPacket);
    }
}

// 发送数据包
void ThreadMgr::SendPacket(Packet *pPacket) {
    NetworkListen* pLocator = static_cast<NetworkListen*>(GetNetwork(APP_Listen));
    pLocator->SendPacket(pPacket);
}

// 释放资源
void ThreadMgr::Dispose() {
    auto iter = _threads.begin();
    while(iter!=_threads.end()){
        Thread* pThread = iter->second;
        pThread->Dispose();
        delete pThread;
        ++iter;
    }
    _threads.clear();
}

// 根据应用类型获取网络对象
Network *ThreadMgr::GetNetwork(APP_TYPE appType) {
    std::lock_guard<std::mutex> guard(_thread_lock);
    auto iter = _networkLocator.find(appType);
    if(iter == _networkLocator.end()) return nullptr;
    return iter->second;
}