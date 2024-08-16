#include "connect_obj.h"

#include "network.h"
#include "network_buffer.h"

#include "packet.h"
#include "thread_mgr.h"

#include "entity_system.h"
#include "message_system_help.h"
#include "system_manager.h"
// 构造函数，接受一个 Network 对象指针和一个 socket 描述符
ConnectObj::ConnectObj() {
  _socket = INVALID_SOCKET;
  _recvBuffer = new RecvNetworkBuffer(DEFAULT_RECV_BUFFER_SIZE, this);
  _sendBuffer = new SendNetworkBuffer(DEFAULT_SEND_BUFFER_SIZE, this);
}

// 析构函数，销毁对象时调用
ConnectObj::~ConnectObj() {
  if (_recvBuffer != nullptr)
    delete _recvBuffer;
  if (_sendBuffer != nullptr)
    delete _sendBuffer;
}

void ConnectObj::AwakeFromPool(SOCKET socket) { _socket = socket; }

void ConnectObj::BackToPool() {
  // std::cout << "close socket:" << _socket << std::endl;

  if (!Global::GetInstance()->IsStop) {
    MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_NetworkDisconnect,
                                      _socket);
  }

  _socket = INVALID_SOCKET;
  _recvBuffer->BackToPool();
  _sendBuffer->BackToPool();
}

// 检查是否有接收的数据
bool ConnectObj::HasRecvData() const { return _recvBuffer->HasData(); }

// 获取接收的数据包
Packet *ConnectObj::GetRecvPacket() const { return _recvBuffer->GetPacket(); }

// 执行接收数据操作
bool ConnectObj::Recv() {
  bool isRs = false;
  char *pBuffer = nullptr;
  while (true) {
    if (_recvBuffer->GetEmptySize() <
        (sizeof(PacketHead) + sizeof(TotalSizeType)))
      _recvBuffer->ReAllocBuffer();
    const int emptySize = _recvBuffer->GetBuffer(pBuffer);
    const int dataSize = ::recv(_socket, pBuffer, emptySize, 0);
    if (dataSize > 0) {
      _recvBuffer->FillData(dataSize);
    } else if (dataSize == 0) {
      break;
    } else {
      const auto socketError = _sock_err();
      if (socketError == EINTR || socketError == EWOULDBLOCK ||
          socketError == EAGAIN) {
        isRs = true;
      }
      break;
    }
  }
  if (isRs) {
    while (true) {
      const auto pPacket = _recvBuffer->GetPacket();
      if (pPacket == nullptr)
        break;

      if (pPacket->GetMsgId() == Proto::MsgId::MI_Ping) {
        // RecvPing();
      } else {
        auto pNetwork = this->GetParent<Network>();
        if (pNetwork->IsBroadcast()) {
          ThreadMgr::GetInstance()->DispatchPacket(pPacket);
        } else {
          GetSystemManager()->GetMessageSystem()->AddPacketToList(pPacket);
        }
      }
    }
  }
  return isRs;
}

// 检查是否有待发送的数据
bool ConnectObj::HasSendData() const { return _sendBuffer->HasData(); }

// 发送数据包
void ConnectObj::SendPacket(Packet *pPacket) const {
  // const google::protobuf::EnumDescriptor *descriptor =
  //     Proto::MsgId_descriptor();
  // auto name = descriptor->FindValueByNumber(pPacket->GetMsgId())->name();
  // std::cout << "send msg:" << name.c_str() << std::endl;

  _sendBuffer->AddPacket(pPacket);
}

// 执行发送数据操作
bool ConnectObj::Send() const {

  while (true) {
    char *pBuffer = nullptr;
    const int needSendSize = _sendBuffer->GetBuffer(pBuffer);

    if (needSendSize <= 0) {
      return true;
    }

    const int size = ::send(_socket, pBuffer, needSendSize, 0);

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
  const auto pPacketDis =
      new Packet(Proto::MsgId::MI_NetworkRequestDisconnect, GetSocket());
  GetSystemManager()->GetMessageSystem()->AddPacketToList(pPacketDis);
}