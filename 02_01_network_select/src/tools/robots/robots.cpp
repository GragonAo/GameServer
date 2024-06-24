#include <atomic> // 引入atomic库，用于原子操作
#include <iostream> // 引入iostream库，用于标准输入输出
#include <thread> // 引入thread库，用于多线程支持
#include <string> // 引入string库，用于字符串处理
#include <vector> // 引入vector库，用于动态数组

#include "client_thread.h" // 引入client_thread头文件，定义了ClientThread类
#include "network/network.h" // 引入network头文件，提供网络相关的功能
#include "network/thread.h" // 引入thread头文件，提供线程相关的功能

// 主函数，程序入口
int main(int argc, char *argv[])
{
    int threadNum = 1, msgCount = 1; // 初始化线程数量和消息数量
    if (argc >= 2) // 如果命令行参数数量大于等于2
    {
        threadNum = std::atoi(argv[1]); // 将第一个参数转换为线程数量
    }

    if (argc == 3) // 如果命令行参数数量等于3
    {
        msgCount = std::atoi(argv[2]); // 将第二个参数转换为消息数量
    }

    std::vector<ClientThread*> threads(threadNum); // 创建一个ClientThread指针的向量，大小为线程数量

    for (auto i = 0; i < threadNum; ++i) // 遍历线程数量
    {
        threads[i] = new ClientThread(msgCount); // 创建新的ClientThread对象
        threads[i]->Start(); // 启动线程
    }

    std::cout << "online socket num:" << threads.size() << "\tcompleted:" << (threadNum - threads.size()) * 100.0f / threadNum << "%" << std::endl;
    // 输出当前在线的socket数量和完成的百分比

    while (true) // 无限循环，直到所有线程完成
    {
        if (threads.size() <= 0) // 如果所有线程都完成
        {
            break; // 跳出循环
        }

        auto iter = threads.begin(); // 获取线程向量的开始迭代器
        while (iter != threads.end()) // 遍历线程向量
        {
            ClientThread* pThread = (*iter); // 获取当前线程指针
            if (!pThread->IsRun()) // 如果线程未运行
            {
                pThread->Dispose(); // 处理线程资源
                delete pThread; // 删除线程对象
                iter = threads.erase(iter); // 从向量中移除线程

                std::cout << "online socket num:" << threads.size() << "\tcompleted:" << (threadNum - threads.size()) * 100.0f / threadNum << "%" << std::endl;
                // 输出当前在线的socket数量和完成的百分比

                continue; // 继续下一次循环
            }

            ++iter; // 移动到下一个线程
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 休眠100毫秒
    }

    threads.clear(); // 清空线程向量
    return 0; // 返回0，表示程序正常结束
}
