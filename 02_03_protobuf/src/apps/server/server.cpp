#include "server.h"
#include "network/network.h"
#include "network/connect_obj.h"
#include "network/network_listen.h"
#include "network/packet.h"
#include "network/protobuf/msg.pb.h"
#include <ostream>
bool Server::DataHandler() {
    for(auto iter = _connects.begin();iter != _connects.end();++iter){
        ConnectObj *pConnectObj = iter->second;
        HandlerOne(pConnectObj);
    }
    if(_isShow){
        _isShow = false;
    }
    return true;
}

void Server::HandlerOne(ConnectObj *pConnectObj) {
    while(pConnectObj->HasRecvData()){
        Packet *pPacket = pConnectObj->GetRecvPacket();
        if(pPacket == nullptr)break;
        Proto::TestMsg protoMsg = pPacket->ParseToProto<Proto::TestMsg>();
        std::cout << "recv msg. msg id:" << pPacket->GetMsgId() << "msg :" <<protoMsg.msg() << "index :" << protoMsg.index() << std::endl;
        pConnectObj->SendPacket(pPacket);
        ++ _recvMsgCount;
        ++ _sendMsgCount;
        _isShow = true;
    }
}
int Server::Accept() {
    int rs = NetworkListen::Accept();
    std::cout << "accept new count:"<<rs <<std::endl;
    _acceptCount += rs;
    _isShow = true;
    return rs;
}