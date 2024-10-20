#pragma once
#include "network/thread.h"

// 预声明 Client 类，避免在这里包含 Client 类的头文件
class Client;

// ClientThread 类继承自 Thread 类，用于处理客户端的线程
class ClientThread : public Thread {
    public:
        // 显式构造函数，接受一个整型参数 msgCount，用于初始化消息计数
        explicit ClientThread(int msgCount);

        // 重载的 Update 函数，在线程运行时会被周期性调用
        void Update() override;

        // 重载的 Dispose 函数，用于释放资源，在线程结束时调用
        void Dispose() override;
    private:
        // 指向 Client 对象的指针，用于与客户端交互
        Client* _pClient{nullptr};

        // 消息计数，用于记录和限制要处理的消息数量
        int _msgCount;
};
