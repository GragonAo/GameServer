#include "login_sync_component.h"
#include "libserver/common.h"
#include "libserver/message_component.h"
#include "libserver/message_system_help.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include <json/writer.h>
#include <sstream>

void LoginSyncComponent::Awake() {

  Json::StreamWriterBuilder jsonBuilder;
  _jsonWrite = jsonBuilder.newStreamWriter();

  auto pMsgCallBack = new MessageCallBackFunction();
  AddComponent<MessageComponent>(pMsgCallBack);

  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_HttpRequestLogin,
      BindFunP1(this, &LoginSyncComponent::HandleHttpRequestLogin));

  pMsgCallBack->RegisterFunction(
      Proto::MsgId::MI_AppInfoSync,
      BindFunP1(this, &LoginSyncComponent::AppInfoSyncHandle));
}

void LoginSyncComponent::BackToPool() { 
  delete _jsonWrite;
  _apps.clear(); 
}

void LoginSyncComponent::HandleHttpRequestLogin(Packet *pPacket) {
  Json::Value responseObj;
  AppInfo info;
  if (!GetOneApp(APP_LOGIN, info)) {
    responseObj["code"] = (int)Proto::LoginHttpreturnCode::LHRC_TIMEOUT;
    responseObj["ip"] = "";
    responseObj["port"] = 0;
  } else {
    responseObj["code"] = (int)Proto::LoginHttpreturnCode::LHRC_OK;
    responseObj["ip"] = info.Ip;
    responseObj["port"] = info.Port;
  }

  std::stringstream jsonStream;
  _jsonWrite->write(responseObj, &jsonStream);

  MessageSystemHelp::SendHttpResponse(pPacket, jsonStream.str().c_str(),
                                      jsonStream.str().length());
}