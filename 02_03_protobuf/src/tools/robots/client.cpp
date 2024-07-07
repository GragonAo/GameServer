#include "client.h"
#include "network/network_connector.h"
#include "network/packet.h"
#include "network/protobuf/msg.pb.h"
#include "network/protobuf/proto_id.pb.h"
#include <random>
#include <sstream>

Client::Client(int msgCount, std::thread::id thread_id) {

  _msgCount = msgCount;
  std::stringstream strStream;
  strStream << thread_id;
  std::string idstr = strStream.str();
  std::seed_seq seed1(idstr.begin(), idstr.end());
  std::minstd_rand0 generator(seed1);
  _pRandomEngine = new std::default_random_engine(generator);
}
bool Client::Connect(std::string ip, int port) {
  if (NetworkConnector::Connect(ip, port)) {
    _lastMsg = "";
    return true;
  }
  return false;
}
void Client::DataHandler() {
if (_isCompleted)
    return;
  if (!IsConnected())
    return;
  if (_index < _msgCount) {
    if (_lastMsg.empty()) {
      _lastMsg = GenRandom();
      Proto::TestMsg protoMsg;
      protoMsg.set_index(_index);
      protoMsg.set_msg(_lastMsg.c_str());
      std::cout << "send msg. size:" << _lastMsg.size() << std::endl;
      Packet packet(Proto::MsgId::SendData);
      packet.SerializeToBuffer(protoMsg);
      SendPacket(&packet);
    } else {
      if (HasRecvData()) {
        Packet *pPacket = GetRecvPacket();
        if (pPacket != nullptr) {
          Proto::TestMsg protoMsg = pPacket->ParseToProto<Proto::TestMsg>();
          std::cout << "recv msg. size:" << protoMsg.msg().size() << std::endl;
          _lastMsg = "";
          ++_index;
          delete pPacket;
        }
      }
    }
  } else {
    _isCompleted = true;
  }
}
std::string Client::GenRandom() {
  std::uniform_int_distribution<int> distribution('a', 'z');
  std::uniform_int_distribution<int> disLength(10, 20);
  int length = disLength(*_pRandomEngine);
  std::string rand_str(length, '\0');
  for (auto &one : rand_str) {
    one = distribution(*_pRandomEngine);
  }
  return rand_str;
}