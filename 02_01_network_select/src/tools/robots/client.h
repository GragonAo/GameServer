#pragma once

#include <list> // 引入list库，用于定义和使用列表容器
#include "network/network_connector.h" // 引入network_connector头文件，提供NetworkConnector基类
#include <random> // 引入random库，用于生成随机数
#include <thread> // 引入thread库，用于线程相关操作

// Client 类继承自 NetworkConnector 类，用于处理客户端网络连接
class Client : public NetworkConnector {
    public:
        // 显式构造函数，接受消息计数和线程ID作为参数
        explicit Client(int msgCount, std::thread::id thread_id);

        // 处理数据的函数，处理接收到的数据
        void DataHandler();

        // 判断操作是否完成的函数，返回是否完成的状态
        bool IsCompleted() const {
            return _isCompleted;
        }

    private:
        // 生成随机字符串的函数
        std::string GenRandom();

    private:
        // 要处理的消息总数
        int _msgCount;

        // 当前处理的消息索引
        int _index{0};

        // 操作是否完成的标志
        bool _isCompleted{false};

        // 上一条消息的内容
        std::string _lastMsg{""};

        // 指向随机数生成引擎的指针，用于生成随机字符串
        std::default_random_engine* _pRandomEngine;
};
