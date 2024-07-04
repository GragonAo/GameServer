#include "server.h"
#include "network/packet.h"
#include <iostream>
#include "network/packet.h"
#include "network/connect_obj.h"
bool Server::DataHandler() {
    for(auto iter = _connects.begin();iter != _connects.end();++iter){
        ConnectObj *pConnectObj = iter->second;
        HandlerOne(pConnectObj);
    }
    if(_isShow){
        _isShow = false;
        std::cout << " accpet: "<<_acceptCount <<std::endl;
    }
    return true;
}

void Server::HandlerOne(ConnectObj *pConnectObj) {
    while(pConnectObj->HasRecvData()){
        Packet *pPacket = pConnectObj->GetRecvPacket();
        if(pPacket == nullptr) break;
        std::string msg(pPacket->GetBuffer(), pPacket->GetDataLength());
		std::cout << "recv size:" << msg.length() << " msg:" << msg.c_str() << std::endl;
        pConnectObj->SendPacket(pPacket);
        ++_recvMsgCount;
        ++_sendMsgCount;
        _isShow = true;
    }
}
int Server::Accept() {
    int rs = NetworkListen::Accept();
    _acceptCount += rs;
    _isShow = true;
    return rs;
}