#pragma once

enum ThreadType {
  MainThread = 1 << 0,
  ListenThread = 1 << 1,
  ConnectThread = 1 << 2,

  LogicThread = 1 << 3,
  MysqlThread = 1 << 4,

  AllThreadType =
      MainThread | ListenThread | ConnectThread | LogicThread | MysqlThread
};

inline const char *GetThreadTypeName(const ThreadType threadType) {
  if (threadType == MainThread)
    return "MainThread";

  if (threadType == LogicThread)
    return "LogicThread";

  if (threadType == ListenThread)
    return "ListenThread";

  if (threadType == ConnectThread)
    return "ConnectThread";

  if (threadType == MysqlThread)
    return "MysqlThread";

  return "unknown";
}