#pragma once

// 包含头文件
#include "libserver/common.h"
#include "libserver/component.h"
#include "libserver/entity.h"
#include "libserver/system.h"
#include <bits/types/struct_timeval.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <json/json.h>
#include <sys/select.h>

// HTTP请求状态的枚举
enum HttpResquestState {
  HRS_Send,       // 发送请求
  HRS_Process,    // 处理请求
  HRS_Over,       // 请求结束
  HRS_NoActive,   // 无活动
  HRS_Timeout,    // 请求超时
};

// HTTP请求方法的枚举
enum HttpResquestMethod { 
  HRM_Post,       // POST请求
  HRM_Get         // GET请求
};

// CURL多处理结果状态的枚举
enum CURLMRS {
  CRS_None,                       // 无状态
  CRS_OK,                         // 成功
  CRS_CURLM_CALL_MULTI_PERFORM,   // 需要调用curl_multi_perform
  CRS_Timeout,                    // 超时
  CRS_CURLMError,                 // CURLM错误
  CRS_SelectError,                // select错误
};

// HttpRequest类，继承自ThreadObject
class HttpRequest : public Entity<HttpRequest>{
public:
  // 更新函数
  void Update();
  void BackToPool() override;
protected:
  // 处理发送请求
  bool ProcessSend();

  // 处理请求超时
  void ProcessTimeout() const;

  // 处理请求结束
  virtual bool ProcessOver();

  // 处理请求
  virtual bool Process();

  // 处理消息
  virtual void ProcessMsg();

  // 处理具体的JSON消息，纯虚函数，子类必须实现
  virtual void ProcessMsg(Json::Value value) = 0;

protected:
  HttpResquestMethod _method{HRM_Get};  // 请求方法，默认为GET
  CURL *_pCurl{nullptr};                // 单个CURL句柄
  CURLM *_pMultiHandle{nullptr};        // 多个CURL句柄管理
  CURLMRS _curlRs;                      // CURL多处理结果状态

  struct curl_slist * _headers{nullptr};

  std::string _url, _params;            // 请求的URL和参数
  std::string _responseBuffer;          // 响应缓冲区

  HttpResquestState _state{ HttpResquestState::HRS_Send };    // 请求状态，默认为发送请求
  std::string _account;                 // 账号信息
};

// 辅助函数，用于执行CURL的多句柄选择操作
inline CURLMRS curl_multi_select(CURLM *curl_m) {
  struct timeval timeout_tv;             // 定义超时时间结构体
  fd_set fd_read;                        // 读文件描述符集合
  fd_set fd_write;                       // 写文件描述符集合
  fd_set fd_except;                      // 异常文件描述符集合
  int max_fd = -1;                       // 文件描述符最大值，初始化为-1
  
  // 清空文件描述符集合
  FD_ZERO(&fd_read);
  FD_ZERO(&fd_write);
  FD_ZERO(&fd_except);

  // 设置超时时间为1秒
  timeout_tv.tv_sec = 10;
  timeout_tv.tv_usec = 0;

  // 获取CURL的文件描述符集合，并更新max_fd
  curl_multi_fdset(curl_m, &fd_read, &fd_write, &fd_except, &max_fd);

  // 如果max_fd为-1，表示没有有效的文件描述符，需要调用curl_multi_perform
  if (max_fd == -1) {
    return CURLMRS::CRS_CURLM_CALL_MULTI_PERFORM;
  }

  // 使用select系统调用等待文件描述符集合中的事件
  int ret_code = ::select(max_fd + 1, &fd_read, &fd_write, &fd_except, &timeout_tv);

  // 根据select的返回值判断事件类型
  switch (ret_code) {
    case -1:    // select调用出错
      return CRS_SelectError;
    case 0:     // select调用超时
      return CRS_Timeout;
    default:    // 有文件描述符可用
      return CRS_OK;
  }
}
