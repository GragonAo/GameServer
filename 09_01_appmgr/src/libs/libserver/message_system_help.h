#pragma once

#include "app_type.h"
#include "common.h"
#include "socket_object.h"
#include <string>

class Packet;

struct http_message;

struct ParseUrlInfo {
  std::string Host;
  int Port;
  std::string Mothed;
  std::string Params;
};

class MessageSystemHelp {
public:
  static Packet *CreatePacket(Proto::MsgId msgId, NetworkIdentify *pIdentify);

  static void DispatchPacket(const Proto::MsgId msgId,
                             NetworkIdentify *pIdentify);
  static void DispatchPacket(const Proto::MsgId msgId,
                             google::protobuf::Message &proto,
                             NetworkIdentify *pIdentify);

  static void SendPacket(const Proto::MsgId msgId, NetworkIdentify *pIdentify,
                         google::protobuf::Message &proto);
  static void SendPacket(const Proto::MsgId msgId,
                         google::protobuf::Message &proto, APP_TYPE appType,
                         int appId = 0);

  static void SendHttpRequest(NetworkIdentify *pIdentify, std::string ip,
                              int port, std::string method,
                              std::map<std::string, std::string>* pParams);

  static void SendHttpResponse(NetworkIdentify *pIdentify, const char *content,
                               int size);
  static void SendHttpResponse404(NetworkIdentify *pIdentify);

  static Packet *ParseHttp(NetworkIdentify *pIdentify, const char *s,
                           unsigned int bodyLen, const bool isChunked,
                           http_message *hm);
  static bool ParseUrl(const std::string &url, ParseUrlInfo &info);

protected:
  static void DispatchPacket(Packet *pPacket);

  static void SendPacket(Packet *pPacket);
  static void SendHttpResponseBase(NetworkIdentify *pIdentify, int status_code,
                                   const char *content, int size);
};