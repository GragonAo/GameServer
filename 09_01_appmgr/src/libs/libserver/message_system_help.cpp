#include "message_system_help.h"
#include "global.h"
#include "common.h"
#include "network_type.h"
#include "socket_object.h"
#include "log4_help.h"
#include "mongoose/mongoose.h"
#include "network_locator.h"
#include "object_pool_packet.h"
#include "packet.h"
#include "thread_mgr.h"
#include <sstream>

void MessageSystemHelp::DispatchPacket(Packet *pPacket) {
  ThreadMgr::GetInstance()->DispatchPacket(pPacket);
}

void MessageSystemHelp::DispatchPacket(const Proto::MsgId msgId,
                                       NetworkIdentify *pIdentify) {
  const auto pPacket = CreatePacket(msgId, pIdentify);
  DispatchPacket(pPacket);
}

void MessageSystemHelp::DispatchPacket(const Proto::MsgId msgId,
                                       google::protobuf::Message &proto,
                                       NetworkIdentify *pIdentify) {
  const auto pPacket = CreatePacket(msgId, pIdentify);
  pPacket->SerializeToBuffer(proto);
  DispatchPacket(pPacket);
}

void MessageSystemHelp::SendPacket(const Proto::MsgId msgId,
                                   NetworkIdentify *pIdentify,
                                   google::protobuf::Message &proto) {
  const auto pPacket = CreatePacket(msgId, pIdentify);
  pPacket->SerializeToBuffer(proto);
  SendPacket(pPacket);
}

void MessageSystemHelp::SendPacket(const Proto::MsgId msgId,
                                   google::protobuf::Message &proto,
                                   APP_TYPE appType, int appId) {
  if ((Global::GetInstance()->GetCurAppType() & appType) != 0) {
    DispatchPacket(msgId, proto, nullptr);
    return;
  }

  auto pNetworkLocator = ThreadMgr::GetInstance()
                             ->GetEntitySystem()
                             ->GetComponent<NetworkLocator>();
  auto networkIdentify = pNetworkLocator->GetNetworkIdentify(appType, appId);
  if (networkIdentify.GetSocketKey().NetType == NetworkType::None) {
    LOG_ERROR("can't find network. appType: " << GetAppName(appType)<< " appId:" << appId);
    return;
  }

  auto packet = CreatePacket(msgId, &networkIdentify);
  packet->SerializeToBuffer(proto);
  SendPacket(packet);
}

void MessageSystemHelp::SendPacket(Packet *pPacket) {

  if (pPacket->GetSocketKey().Socket == INVALID_SOCKET ||
      pPacket->GetSocketKey().NetType == NetworkType::None) {
    DispatchPacket(pPacket);
    return;
  }

  auto pNetworkLocator = ThreadMgr::GetInstance()
                             ->GetEntitySystem()
                             ->GetComponent<NetworkLocator>();
  const auto socketKey = pPacket->GetSocketKey();
  const auto networkType = socketKey.NetType;
  if (networkType == NetworkType::TcpConnector ||
      networkType == NetworkType::HttpConnector) {
    auto pNetwork = pNetworkLocator->GetConnector(networkType);
    if (pNetwork != nullptr) {
      pNetwork->SendPacket(pPacket);
      return;
    }
  } else {
    auto pNetwork = pNetworkLocator->GetListen(networkType);
    if (pNetwork != nullptr) {
      pNetwork->SendPacket(pPacket);
      return;
    }
  }
  LOG_ERROR("failed to send packet. "
            << dynamic_cast<NetworkIdentify *>(pPacket));
}

Packet *MessageSystemHelp::CreatePacket(Proto::MsgId msgId,
                                        NetworkIdentify *pIdentify) {
  return DynamicPacketPool::GetInstance()->MallocPacket(msgId, pIdentify);
}

void MessageSystemHelp::SendHttpResponseBase(NetworkIdentify *pIdentify,
                                             int status_code,
                                             const char *content, int size) {
  auto pNetworkLocator = ThreadMgr::GetInstance()
                             ->GetEntitySystem()
                             ->GetComponent<NetworkLocator>();
  auto pNetwork = pNetworkLocator->GetListen(NetworkType::HttpListen);
  if (pNetwork == nullptr) {
    LOG_ERROR("can't find network. http send failed. socket: "
              << pIdentify);
    return;
  }
  Packet *pPacket = CreatePacket(Proto::MsgId::MI_HttpInnerResponse, pIdentify);

  std::stringstream buffer;
  buffer << "HTTP/1.1 " << status_code << " " << mg_status_message(status_code)
         << "\r\n";
  buffer << "Connection: close\r\n";
  buffer << "Content-Type: application/json; charset=utf-8\r\n";
  buffer << "Content-Length:" << size << "\r\n\r\n";
  if (size > 0)
    buffer.write(content, size);

  pPacket->SerializeToBuffer(buffer.str().c_str(), buffer.tellg());

#if LOG_HTTP_OPEN
  LOG_HTTP("\r\n" << std::string(buffer.str().c_str(), buffer.tellp()).c_str());
#endif

  pNetwork->SendPacket(pPacket);
}

void MessageSystemHelp::SendHttpResponse404(NetworkIdentify *pIdentify) {
  SendHttpResponseBase(pIdentify, 404, nullptr, 0);
}

void MessageSystemHelp::SendHttpResponse(NetworkIdentify *pIdentify,
                                         const char *content, int size) {
  SendHttpResponseBase(pIdentify, 200, content, size);
}

bool MessageSystemHelp::ParseUrl(const std::string &url, ParseUrlInfo &info) {
  struct mg_str uri;
  uri.p = url.c_str();
  uri.len = url.length();

  unsigned int port = 0;
  struct mg_str scheme, user_info, host, path, query, fragment;
  mg_parse_uri(uri, &scheme, &user_info, &host, &port, &path, &query,
               &fragment);

  info.Host = std::string(host.p, host.len);
  info.Port = port == 0 ? 80 : port;
  info.Mothed = std::string(path.p, path.len);
  info.Params = std::string(query.p, query.len);

  return true;
}

Packet *MessageSystemHelp::ParseHttp(NetworkIdentify *pIdentify, const char *s,
                                     unsigned int bodyLen, const bool isChunked,
                                     http_message *hm) {
  Proto::Http proto;
  if (bodyLen > 0) {
    if (isChunked) {
      const auto end = s + bodyLen;
      mg_str tmp;
      while (true) {
        const int len = std::atoi(s);
        if (len == 0)
          break;
        s = mg_skip(s, end, "\r\n", &tmp);
        s = mg_skip(s, end, "\r\n", &tmp);
        proto.mutable_body()->append(tmp.p, tmp.len);
      }
    } else {
      proto.mutable_body()->append(s, bodyLen);
    }
  }

  Proto::MsgId msgId = Proto::MsgId::MI_HttpRequestBad;
  if (hm->method.len > 0) {
    do {
      if (mg_vcasecmp(&hm->method, "GET") == 0) {
        if (mg_vcasecmp(&hm->uri, "/login") == 0) {
          msgId = Proto::MsgId::MI_HttpRequestLogin;
          break;
        }
      }
      LOG_ERROR("recv http mothod. "
                << " mothod:"
                << std::string(hm->method.p, hm->method.len).c_str()
                << " len:" << hm->method.len
                << " uri:" << std::string(hm->uri.p, hm->uri.len).c_str()
                << " len:" << hm->uri.len);
    } while (false);
  } else {
    proto.set_status_code(hm->resp_code);
    msgId = Proto::MsgId::MI_HttpOuterResponse;
  }

  auto pPacket = CreatePacket(msgId, pIdentify);
  pPacket->SerializeToBuffer(proto);
  return pPacket;
}

void MessageSystemHelp::SendHttpRequest(
    NetworkIdentify *pIdentify, std::string ip, int port, std::string method,
    std::map<std::string, std::string> *pParams) {
  
  Packet *pPacket = CreatePacket(Proto::MsgId::MI_HttpOuterRequest, pIdentify);
  
  std::stringstream buffer;
  buffer << "GET " << method;
  
  if (pParams != nullptr) {
    buffer << "?";
    for (auto iter = pParams->begin(); iter != pParams->end(); ++iter) {
      buffer << iter->first << "=" << iter->second << "&";
    }
  }
  buffer << " HTTP/1.1" << "\r\n";
  buffer << "Host: " << ip.c_str() << ":" << port << "\r\n";
  buffer << "Content-Type: application/json; charset=utf-8" << "\r\n";
  buffer << "Accept: */*,text/*,text/html" << "\r\n";
  buffer << "\r\n";
  // tellp : 获取当前位置：tellp() 返回一个 std::streampos
  // 类型的值，表示当前的输出位置。可以用来判断当前已写入多少数据。
  pPacket->SerializeToBuffer(buffer.str().c_str(), buffer.tellp());
  
#if LOG_HTTP_OPEN
  auto msg = buffer.str();
  LOG_HTTP("\r\n" << std::string(msg.data(), buffer.tellp()).c_str());
#endif

  SendPacket(pPacket);
}