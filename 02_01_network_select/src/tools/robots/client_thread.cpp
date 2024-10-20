#include <iostream> // 引入iostream库，用于标准输入输出
#include "client.h" // 引入client头文件，定义了Client类
#include "client_thread.h" // 引入client_thread头文件，声明了ClientThread类

// 构造函数，接受消息计数作为参数
ClientThread::ClientThread(int msgCount) {
    _msgCount = msgCount; // 初始化消息计数
}

// 线程主循环函数
void ClientThread::Update() {
    // 如果客户端对象为空，创建新的客户端对象并尝试连接
    if (_pClient == nullptr) {
        _pClient = new Client(_msgCount, _thread.get_id()); // 创建新的Client对象
        if (!_pClient->Connect("127.0.0.1", 2233)) { // 尝试连接到服务器
            std::cout << "ClientThread Connect failed." << std::endl; // 连接失败输出错误信息
        }
    } else { // 如果客户端对象已存在
        _pClient->Update(); // 更新客户端状态
        _pClient->DataHandler(); // 处理接收到的数据
        if (_pClient->IsCompleted()) { // 如果客户端操作完成
            Thread::Stop(); // 停止线程
        }
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 线程休眠10毫秒
}

// 清理资源函数
void ClientThread::Dispose() {
    if (_pClient != nullptr) { // 如果客户端对象不为空
        _pClient->Dispose(); // 清理客户端资源
        delete _pClient; // 释放客户端对象内存
        _pClient = nullptr; // 置空客户端指针
    }
    Thread::Dispose(); // 调用基类的Dispose函数，清理线程相关资源
}
