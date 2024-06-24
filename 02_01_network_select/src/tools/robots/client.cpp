#include <thread> // 引入thread库，用于多线程支持
#include <iostream> // 引入iostream库，用于标准输入输出
#include "client.h" // 引入Client头文件，定义了Client类
#include "network/packet.h" // 引入packet头文件，定义了Packet类
#include <sstream> // 引入sstream库，用于字符串流操作
#include <random> // 引入random库，用于生成随机数

// 构造函数，接受消息计数和线程ID作为参数
Client::Client(int msgCount, std::thread::id threadId) {
    _msgCount = msgCount; // 初始化消息计数
    std::stringstream strStream; // 创建字符串流
    strStream << threadId; // 将线程ID写入字符串流
    std::string idstr = strStream.str(); // 获取线程ID的字符串表示
    std::seed_seq seed1(idstr.begin(), idstr.end()); // 使用线程ID字符串创建种子序列
    std::minstd_rand0 generator(seed1); // 使用种子序列初始化随机数生成器

    _pRandomEngine = new std::default_random_engine(generator()); // 创建默认随机数引擎
}

// 数据处理函数
void Client::DataHandler() {
    if (_isCompleted) return; // 如果操作已完成，直接返回
    if (!IsConnected()) return; // 如果未连接，直接返回

    if (_index < _msgCount) { // 如果未达到消息计数
        if (_lastMsg.empty()) { // 如果上次消息为空，生成并发送新的消息
            _lastMsg = GenRandom(); // 生成随机消息
            std::cout << "send. size:" << _lastMsg.length() << " msg:" << _lastMsg.c_str() << std::endl; // 输出发送的消息信息
            Packet* pPacket = new Packet(1); // 创建新的Packet对象
            pPacket->AddBuffer(_lastMsg.c_str(), _lastMsg.length()); // 将消息添加到Packet
            SendPacket(pPacket); // 发送Packet
            delete pPacket; // 删除Packet对象
        } else { // 如果上次消息不为空，检查是否有接收到的数据
            if (HasRecvData()) { // 检查是否有接收到的数据
                Packet* pPacket = GetRecvPacket(); // 获取接收到的Packet
                if (pPacket != nullptr) { // 如果Packet不为空
                    const std::string msg(pPacket->GetBuffer(), pPacket->GetDataLength()); // 从Packet中获取消息
                    std::cout << "recv. size:" << pPacket->GetDataLength() << std::endl; // 输出接收到的消息信息
                    if (msg != _lastMsg) // 如果接收到的消息与发送的消息不一致
                        std::cout << " !!!!!!! error." << std::endl; // 输出错误信息
                    _lastMsg = ""; // 清空上次消息
                    ++_index; // 增加消息计数
                    delete pPacket; // 删除Packet对象
                }
            }
        }
    } else { // 如果已达到消息计数，标记操作完成
        _isCompleted = true;
    }
}

// 生成随机字符串函数
std::string Client::GenRandom() {
    std::uniform_int_distribution<int> distribution{ 'a', 'z' }; // 定义字符分布范围
    std::uniform_int_distribution<int> disLength(10, 512); // 定义字符串长度分布范围
    int length = disLength(*_pRandomEngine); // 生成随机长度

    std::string rand_str(length, '\0'); // 创建指定长度的字符串
    for (auto& one : rand_str) { // 遍历字符串
        one = distribution(*_pRandomEngine); // 为每个字符生成随机值
    }

    return rand_str; // 返回生成的随机字符串
}
