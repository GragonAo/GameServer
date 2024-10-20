#include "test_http_login.h"
#include "libserver/message_system_help.h"
#include "libserver/component_help.h"
#include "libserver/message_system.h"

void TestHttpLogin::Awake(std::string account, std::string password)
{
    _account = account;  // 初始化账户信息
    _password = password;  // 初始化密码信息

    // 设置对象的唯一标识（基于账户信息）
    _objKey = { ObjectKeyType::Account , {0, _account} };

    // 注册消息回调函数，用于处理网络相关事件
    // auto pMsgCallBack = new MessageCallBackFunctionFilterObj<TestHttpLogin>();
    // AddComponent<MessageComponent>(pMsgCallBack);  // 添加消息组件，用于管理消息处理

    // // 指定获取包中对象的函数，用于确保消息被正确路由到对应对象
    // pMsgCallBack->GetPacketObject = [this](NetworkIdentify* pIdentify)
    // {
    //     if (_objKey == pIdentify->GetObjectKey())  // 比较对象标识
    //         return this;

    //     return static_cast<TestHttpLogin*>(nullptr);  // 如果不匹配则返回空指针
    // };

    // // 注册处理网络断开、连接和 HTTP 响应的消息回调
    // pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_NetworkDisconnect, BindFunP2(this, &TestHttpLogin::HandleNetworkDisconnect));
    // pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_NetworkConnected, BindFunP2(this, &TestHttpLogin::HandleNetworkConnected));
    // pMsgCallBack->RegisterFunctionWithObj(Proto::MsgId::MI_HttpOuterResponse, BindFunP2(this, &TestHttpLogin::HandleHttpOuterResponse));

    // 获取配置文件中的登录 URL
    auto pYaml = ComponentHelp::GetYaml();
    const auto pLoginConfig = dynamic_cast<LoginConfig*>(pYaml->GetConfig(APP_LOGIN));
    
    // 解析登录 URL，如果解析失败则打印错误日志
    ParseUrlInfo info;
    if (!MessageSystemHelp::ParseUrl(pLoginConfig->UrlLogin, info))
    {
        LOG_ERROR("parse login url failed. url:" << pLoginConfig->UrlLogin.c_str());
        return;
    }

    // 提取并保存 URL 中的 IP、端口和 HTTP 方法
    _ip = info.Host;
    _port = info.Port;
    _mothed = pLoginConfig->UrlMethod;
    _path = info.Path;

    // 构造网络连接请求并发送，连接到登录服务器
    Proto::NetworkConnect protoConn;
    protoConn.set_network_type((int)NetworkType::HttpConnector);  // 指定网络类型为 HTTP 连接器
    _objKey.SerializeToProto(protoConn.mutable_key());  // 将对象标识序列化
    protoConn.set_ip(_ip.c_str());  // 设置目标 IP
    protoConn.set_port(_port);  // 设置目标端口
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnect, protoConn, nullptr);  // 发送连接请求
}

void TestHttpLogin::BackToPool()
{
    // 在回收对象到对象池时，执行任何必要的清理操作（此处未实现具体清理逻辑）
}

void TestHttpLogin::HandleNetworkDisconnect(TestHttpLogin* pObj, Packet* pPacket)
{
    // 处理网络断开事件，记录错误日志
    LOG_ERROR("test http login. recv network disconnect.");
}

void TestHttpLogin::HandleNetworkConnected(TestHttpLogin* pObj, Packet* pPacket)
{
    // 处理网络连接成功事件，保存 socket 信息
    _socketKey = pPacket->GetSocketKey();
    LOG_DEBUG("connected." << pPacket);

    // 构造 HTTP 请求参数并发送登录请求
    std::map<std::string, std::string> params;
    params["account"] = _account;  // 设置账号参数
    params["password"] = _password;  // 设置密码参数
    MessageSystemHelp::SendHttpRequest(this, _ip, _port, _mothed,_path, &params);  // 发送 HTTP 请求
}

void TestHttpLogin::HandleHttpOuterResponse(TestHttpLogin* pObj, Packet* pPacket)
{
    // 处理 HTTP 响应，解析并打印响应状态码和响应体
    auto protoHttp = pPacket->ParseToProto<Proto::Http>();
    LOG_DEBUG("http code:" << protoHttp.status_code());
    LOG_DEBUG(protoHttp.body().c_str());
}
