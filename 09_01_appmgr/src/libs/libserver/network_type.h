#pragma once

enum class NetworkType {
  None = 0,
  TcpListen = 1 << 0,
  TcpConnector = 1 << 1,
  HttpListen = 1 << 2,
  HttpConnector = 1 << 3,
};

inline const char *GetNetworkTypeName(const NetworkType appType) {
  switch (appType) {
  case NetworkType::TcpListen:
    return "TcpListen";
  case NetworkType::TcpConnector:
    return "TcpConnector";
  case NetworkType::HttpListen:
    return "HttpListen";
  case NetworkType::HttpConnector:
    return "HttpConnector";
  default:
    return "None";
  }
  return "None";
}