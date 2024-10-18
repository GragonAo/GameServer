#pragma once
#include <mutex>
#include <map>
#include <list>
#include <functional>

#include "common.h"
#include "packet.h"

// 消息回调函数接口
class IMessageCallBackFunction
{
public:
    virtual ~IMessageCallBackFunction() = default;

    // 判断是否处理该消息ID
    virtual bool IsFollowMsgId(Packet* packet) = 0;

    // 处理收到的消息
    virtual void ProcessPacket(Packet* packet) = 0;
};

// 消息回调函数实现类
class MessageCallBackFunction : public IMessageCallBackFunction
{
public:
    // 定义处理函数的类型
    using HandleFunction = std::function<void(Packet*)>;

    // 注册处理函数
    void RegisterFunction(int msgId, HandleFunction function);

    // 判断是否处理该消息ID
    bool IsFollowMsgId(Packet* packet) override;

    // 处理收到的消息
    void ProcessPacket(Packet* packet) override;

    // 获取回调处理器
    std::map<int, HandleFunction>& GetCallBackHandler() { return _callbackHandle; }

protected:
    // 存储消息ID与处理函数的映射
    std::map<int, HandleFunction> _callbackHandle;
};

// 消息回调函数带对象过滤器实现类
template<class T>
class MessageCallBackFunctionFilterObj : public MessageCallBackFunction
{
public:
    // 定义带对象的处理函数类型
    using HandleFunctionWithObj = std::function<void(T*, Packet*)>;
    using HandleGetObject = std::function<T*(NetworkIdentify*)>;

    // 注册带对象的处理函数
    void RegisterFunctionWithObj(int msgId, HandleFunctionWithObj function);

    // 判断是否处理该消息ID
    bool IsFollowMsgId(Packet* packet) override;

    // 处理收到的消息
    void ProcessPacket(Packet* packet) override;

    // 获取对象的函数指针，默认为nullptr
    HandleGetObject GetPacketObject{ nullptr };

private:
    // 存储带对象的消息ID与处理函数的映射
    std::map<int, HandleFunctionWithObj> _callbackHandleWithObj;
};

// 注册带对象的处理函数
template <class T>
void MessageCallBackFunctionFilterObj<T>::RegisterFunctionWithObj(const int msgId, HandleFunctionWithObj function)
{
    _callbackHandleWithObj[msgId] = function;
}

// 判断是否处理该消息ID
template <class T>
bool MessageCallBackFunctionFilterObj<T>::IsFollowMsgId(Packet* packet)
{
    // 首先检查基类是否处理该消息ID
    if (MessageCallBackFunction::IsFollowMsgId(packet))
        return true;

    // 检查是否有带对象的处理函数
    if (_callbackHandleWithObj.find(packet->GetMsgId()) != _callbackHandleWithObj.end())
    {
        if (GetPacketObject != nullptr)
        {
            // 获取与消息对应的对象
            T* pObj = GetPacketObject(packet);
            if (pObj != nullptr)
                return true;
        }
    }

    return false;
}

// 处理收到的消息
template <class T>
void MessageCallBackFunctionFilterObj<T>::ProcessPacket(Packet* packet)
{
    // 查找对应的处理函数
    const auto handleIter = _callbackHandle.find(packet->GetMsgId());
    if (handleIter != _callbackHandle.end())
    {
        handleIter->second(packet); // 调用处理函数
        return;
    }

    // 查找带对象的处理函数
    auto iter = _callbackHandleWithObj.find(packet->GetMsgId());
    if (iter != _callbackHandleWithObj.end())
    {
        if (GetPacketObject != nullptr)
        {
            // 获取与消息对应的对象
            T* pObj = GetPacketObject(packet);
            if (pObj != nullptr)
            {
                iter->second(pObj, packet); // 调用带对象的处理函数
            }
        }
    }
}
