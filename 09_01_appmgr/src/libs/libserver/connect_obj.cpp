#include "connect_obj.h"

#include "common.h"
#include "log4_help.h"
#include "network_help.h"
#include "network_type.h"
#include "protobuf/proto_id.pb.h"
#include "socket_object.h"
#include "network.h"
#include "network_buffer.h"

#include "packet.h"
#include "thread_mgr.h"

#include "entity_system.h"
#include "message_system_help.h"
#include "system_manager.h"
#include "object_pool_packet.h"
#include "component_help.h"
#include "network_locator.h"

// 构造函数，接受一个 Network 对象指针和一个 socket 描述符
ConnectObj::ConnectObj() {
  _state = ConnectStateType::None;
  _recvBuffer = new RecvNetworkBuffer(DEFAULT_RECV_BUFFER_SIZE, this);
  _sendBuffer = new SendNetworkBuffer(DEFAULT_SEND_BUFFER_SIZE, this);
}

// 析构函数，销毁对象时调用
ConnectObj::~ConnectObj() {
    delete _recvBuffer;
    delete _sendBuffer;
}

void ConnectObj::Awake(SOCKET socket,NetworkType networkType,ObjectKey key,ConnectStateType state) { 
  _socketKey = SocketKey(socket,networkType);
  _objKey = key;
  _state = state;
}

void ConnectObj::BackToPool() {

#ifdef LOG_TRACE_COMPONENT_OPEN //追溯连接对象的关闭
  const auto traceMsg = std::string("close. network type: ").append(GetNetworkTypeName(_socketKey.NetType));
  ComponentHelp::GetTraceComponent()->Trace(TraceType::Connector,_socketKey.Socket,traceMsg);
#endif

  if(GetObjectKey().KeyType == ObjectKeyType::App){ //连接对象类型为App，从网络定位组件移除
    auto pLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
    pLocator->RemoveNetworkIdentify(GetObjectKey().KeyValue.KeyInt64);
  }else{  //其他直接发送连接断开协议
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkDisconnect,this);
  }

  if(_socketKey.Socket != INVALID_SOCKET)
    _sock_close(_socketKey.Socket); //关闭Sokcet

  _state = ConnectStateType::None;
  _socketKey.Clean();
  _objKey.Clean();
  
  _recvBuffer->BackToPool();
  _sendBuffer->BackToPool();
}

// 检查接受缓冲区是否有接收的数据
bool ConnectObj::HasRecvData() const { return _recvBuffer->HasData(); }

// 接收网络层数据操作
bool ConnectObj::Recv() {

  //能接受到消息必然是已经处于连接状态了，若没有则修改状态到已连接状态
  if(_state == ConnectStateType::Connecting){ 
    ChangeStateToConnected();
    return true;
  }

  bool isRs = false;
  char *pBuffer = nullptr;
  while (true) {
    //若接受缓冲区空间不足，进行缓冲区重新分配
    if (_recvBuffer->GetEmptySize() <
        (sizeof(PacketHead) + sizeof(TotalSizeType)))
      _recvBuffer->ReAllocBuffer();

    const int emptySize = _recvBuffer->GetBuffer(pBuffer);
    //从网络层读取数据,一直读到当前socket的recv读不到数据为止
    const int dataSize = ::recv(_socketKey.Socket, pBuffer, emptySize, 0);
    if (dataSize > 0) {
      //读取到了数据进行字节容量填充
      _recvBuffer->FillData(dataSize);
      isRs = true;
    } else if (dataSize == 0) {
      break;
    } else {
      const auto socketError = _sock_err();
      isRs = socketError != 0;
      if(!NetworkHelp::IsError(socketError)){
        isRs = true;
      }

      if(!isRs){
        LOG_WARN("recv size: " << dataSize << " error: " << socketError);
        break;
      }
    }
  }
  
  //如果本次有读取到完整数据
  if (isRs) {
    const auto pNetwork = this->GetParent<Network>();
    const auto iNetworkType = pNetwork->GetNetworkType();
    while (true) {
      const auto pPacket = _recvBuffer->GetPacket();
      if (pPacket == nullptr)
        break;
      
      const auto msgId = pPacket->GetMsgId();
      const bool isTcp = NetworkHelp::IsTcp(iNetworkType);

      if(!isTcp){
        if(msgId == Proto::MsgId::MI_HttpRequestBad){
          MessageSystemHelp::SendHttpResponse404(pPacket);
          DynamicPacketPool::GetInstance()->FreeObject(pPacket);
          continue;
        }
      }else{
        if(msgId == Proto::MsgId::MI_Ping){
          //TODO 心跳
          continue;
        }
      }

      if(!isTcp){
        if((msgId <= Proto::MsgId::MI_HttpBegin || msgId >= Proto::MsgId::MI_HttpEnd) && msgId != Proto::MsgId::MI_HttpOuterResponse){
            LOG_WARN("http connect recv. tcp proto");
            DynamicPacketPool::GetInstance()->FreeObject(pPacket);
            continue;
        }
      }
      ThreadMgr::GetInstance()->DispatchPacket(pPacket);      
    }
  }
  return isRs;
}

// 检查发送缓冲区是否有待发送的数据
bool ConnectObj::HasSendData() const { return _sendBuffer->HasData(); }

// 发送数据包
void ConnectObj::SendPacket(Packet *pPacket) const {
  _sendBuffer->AddPacket(pPacket);  //将packet添加进发送缓冲区buffer中
  DynamicPacketPool::GetInstance()->FreeObject(pPacket);
}

// 执行发送数据操作
bool ConnectObj::Send(){

  if(_state == ConnectStateType::Connecting){
    ChangeStateToConnected();
    return true;
  }

  while (true) {
    char *pBuffer = nullptr;
    const int needSendSize = _sendBuffer->GetBuffer(pBuffer); //获取需要发送的缓冲区大小

    if (needSendSize <= 0) {
      return true;
    }
    //发送到网络层的发送缓冲区中
    const int size = ::send(_socketKey.Socket, pBuffer, needSendSize, 0);

    if (size > 0) {
      _sendBuffer->RemoveData(size);

      if (size < needSendSize) {
        return true;
      }
    }

    if (size <= 0) {
      const auto socketErro = _sock_err();
      std::cout << "needSendSize:" << needSendSize << " error:" << socketErro
                << std::endl;
      return false;
    }
  }
}

// 关闭连接
void ConnectObj::Close() {
  //
  const auto pPacketDis =
      MessageSystemHelp::CreatePacket(Proto::MsgId::MI_NetworkRequestDisconnect, this);
  GetSystemManager()->GetMessageSystem()->AddPacketToList(pPacketDis);
  pPacketDis->OpenRef();
}

ConnectStateType ConnectObj::GetState() const{
  return _state;
}

void ConnectObj::ChangeStateToConnected(){
  _state = ConnectStateType::Connected;
  if(GetObjectKey().KeyType == ObjectKeyType::App){
    auto pLocator = ThreadMgr::GetInstance()->GetEntitySystem()->GetComponent<NetworkLocator>();
    pLocator->AddNetworkIdentify(GetObjectKey().KeyValue.KeyInt64,GetSocketKey(),GetObjectKey());
  }else{
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkConnected,this);
  }
}

void ConnectObj::ModifyConnectKey(ObjectKey key){
  _objKey = key;
}