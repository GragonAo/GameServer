#include "network/network_buffer.h" // 引入network_buffer头文件，定义了SendNetworkBuffer和RecvNetworkBuffer类
#include <string> // 引入string库，用于字符串处理
#include <cstring> // 引入cstring库，用于内存操作
#include "network/packet.h" // 引入packet头文件，定义了Packet类
#include <algorithm> // 引入algorithm库，用于标准算法
#include <iostream> // 引入iostream库，用于标准输入输出

// 发送数据函数
void SendData(SendNetworkBuffer* pSendbuff, RecvNetworkBuffer* pRecvbuff)
{
    char* pSendBuffer = nullptr; // 发送缓冲区指针
    char* pRecvBuffer = nullptr; // 接收缓冲区指针

    // 不断发送数据，直到发送缓冲区没有数据
    while (true)
    {
        const int needSendSize = pSendbuff->GetBuffer(pSendBuffer); // 获取需要发送的数据大小
        if (needSendSize <= 0) // 如果没有数据需要发送，则退出循环
            break;

        // 如果接收缓冲区剩余空间不足以接收一个完整的数据包，则重新分配缓冲区
        if (pRecvbuff->GetEmptySize() < (sizeof(PacketHead) + sizeof(TotalSizeType)))
        {
            pRecvbuff->ReAllocBuffer();
        }

        int emptySize = pRecvbuff->GetBuffer(pRecvBuffer); // 获取接收缓冲区剩余空间大小
        const int mixSize = std::min(needSendSize, emptySize); // 计算实际要发送的数据大小
        ::memcpy(pRecvBuffer, pSendBuffer, mixSize); // 将发送缓冲区的数据复制到接收缓冲区

        pSendbuff->RemoveData(mixSize); // 从发送缓冲区中移除已发送的数据
        pRecvbuff->FillData(mixSize); // 填充接收缓冲区
    }
}

// 接收数据函数
void RecvData(RecvNetworkBuffer* pRecvbuff)
{
    // 不断接收数据，直到接收缓冲区没有数据
    while (pRecvbuff->HasData())
    {
        const auto pPacket = pRecvbuff->GetPacket(); // 获取一个数据包
        std::string msg(pPacket->GetBuffer(), pPacket->GetDataLength()); // 将数据包内容转换为字符串
        std::cout << "recv msg:" << msg.c_str() << std::endl; // 输出接收到的消息
    }
}

// 测试发送缓冲区1
void TestSendBuffer1()
{
    // 创建发送缓冲区和接收缓冲区
    SendNetworkBuffer sendbuff(DEFAULT_SEND_BUFFER_SIZE);
    RecvNetworkBuffer recvbuff(DEFAULT_RECV_BUFFER_SIZE);

    std::string msg = "123456"; // 定义要发送的消息

    // 创建一个数据包并添加消息
    Packet packet(1);
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet); // 将数据包添加到发送缓冲区

    // 发送第二个数据包
    msg = "654321";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    // 发送第三个数据包
    msg = "111223";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 再次发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    std::cout << "************** case over *******************" << std::endl; // 输出测试结束标志
}

// 测试发送缓冲区2
void TestSendBuffer2()
{
    // 创建大容量的发送缓冲区和接收缓冲区
    SendNetworkBuffer sendbuff(DEFAULT_SEND_BUFFER_SIZE * 2);
    RecvNetworkBuffer recvbuff(DEFAULT_RECV_BUFFER_SIZE * 2);

    std::string msg = "12345"; // 定义要发送的消息

    // 创建一个数据包并添加消息
    Packet packet(1);
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet); // 将数据包添加到发送缓冲区

    // 发送第二个数据包
    msg = "123456";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    // 发送第三个数据包
    msg = "654321";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 再次发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    std::cout << "************** case over *******************" << std::endl; // 输出测试结束标志
}

// 测试发送缓冲区3
void TestSendBuffer3()
{
    // 创建大容量的发送缓冲区和接收缓冲区
    SendNetworkBuffer sendbuff(DEFAULT_SEND_BUFFER_SIZE * 2);
    RecvNetworkBuffer recvbuff(DEFAULT_RECV_BUFFER_SIZE * 2);

    std::string msg = "123"; // 定义要发送的消息

    // 创建一个数据包并添加消息
    Packet packet(1);
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet); // 将数据包添加到发送缓冲区

    // 发送第二个数据包
    msg = "654321";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    // 发送第三个数据包
    msg = "123456";
    packet.Dispose();
    packet.AddBuffer(msg.c_str(), msg.length());
    sendbuff.AddPacket(&packet);

    // 再次发送数据并接收数据
    SendData(&sendbuff, &recvbuff);
    RecvData(&recvbuff);

    // 输出发送缓冲区和接收缓冲区的信息
    std::cout << "send buffer. begin index:" << sendbuff.GetBeginIndex() << " end index:" << sendbuff.GetEndIndex() << " total size:" << sendbuff.GetTotalSize() << std::endl;
    std::cout << "recv buffer. begin index:" << recvbuff.GetBeginIndex() << " end index:" << recvbuff.GetEndIndex() << " total size:" << recvbuff.GetTotalSize() << std::endl;

    std::cout << "************** case over *******************" << std::endl; // 输出测试结束标志
}

// 主函数
int main(int argc, char *argv[])
{
    TestSendBuffer1(); // 运行测试发送缓冲区1
    TestSendBuffer2(); // 运行测试发送缓冲区2
    TestSendBuffer3(); // 运行测试发送缓冲区3
    return 0; // 返回0，表示程序正常结束
}
