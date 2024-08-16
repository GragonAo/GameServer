#include "http_request.h"
#include "libserver/message_system_help.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include <curl/easy.h>
#include <curl/multi.h>
#include <iostream>
#include <json/config.h>
#include <json/reader.h>
#include <json/value.h>

// 写入数据的回调函数，用于将CURL的响应数据写入字符串
size_t WriteFunction(void *buffer, size_t size, size_t nmemb, void *lpVoid) {
  std::string *str = static_cast<std::string *>(lpVoid);
  if (nullptr == str || nullptr == buffer)
    return -1; // 如果指针为空，返回错误
  char *pData = static_cast<char *>(buffer);
  str->append(pData, size * nmemb); // 将数据追加到字符串
  return size * nmemb;              // 返回写入的数据大小
}

void HttpRequest::BackToPool() {
  _responseBuffer = "";
  State = HttpResquestState::HRS_Send;

  if (_pMultiHandle != nullptr && _pCurl != nullptr)
    curl_multi_remove_handle(_pMultiHandle, _pCurl);

  if (_pCurl != nullptr)
    curl_easy_cleanup(_pCurl);

  if (_pMultiHandle != nullptr)
    curl_multi_cleanup(_pMultiHandle);

  _pCurl = nullptr;
  _pMultiHandle = nullptr;
}

// 更新函数，根据当前状态处理不同逻辑
void HttpRequest::Update() {
  switch (State) {
  case HRS_Send:
    if (ProcessSend()) // 如果处理发送成功，更新状态为处理请求
      State = HRS_Process;
    break;
  case HRS_Process:
    if (Process()) // 如果处理请求成功，更新状态为请求结束
      State = HRS_Over;
    break;
  case HRS_Over:
    ProcessOver();        // 处理请求结束
    State = HRS_NoActive; // 更新状态为无活动
    _active = false;      // 设置活动状态为false
    break;
  case HRS_Timeout:
    ProcessTimeout(); // 处理请求超时
    State = HRS_Over;
    break;

  default:
    break;
  }
}

// 处理发送请求
bool HttpRequest::ProcessSend() {

  _pCurl = curl_easy_init();
  _pMultiHandle = curl_multi_init();

  // 设置CURL请求的URL
  curl_easy_setopt(_pCurl, CURLOPT_URL, _url.c_str());
  curl_easy_setopt(_pCurl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION,
                   WriteFunction); // 设置写入数据的回调函数
  curl_easy_setopt(
      _pCurl, CURLOPT_WRITEDATA,
      static_cast<void *>(&_responseBuffer)); // 设置写入数据的缓冲区

  curl_easy_setopt(_pCurl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(_pCurl, CURLOPT_VERBOSE, 0);
  curl_easy_setopt(_pCurl, CURLOPT_HEADER, 0);

  // 设置Content-Type头为application/json
  struct curl_slist *headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, headers);

  // 将CURL句柄添加到多句柄管理器中
  curl_multi_add_handle(_pMultiHandle, _pCurl);

  // 如果请求方法为POST，设置POST参数
  if (_method == HttpResquestMethod::HRM_Post) {
    curl_easy_setopt(_pCurl, CURLOPT_POST, true);
    curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, _params.c_str());
  }

  State = HttpResquestState::HRS_Process; // 更新状态为处理请求
  // std::cout << "向Java服务器发送登录请求" << std::endl;
  return true;
}

// 处理请求超时
void HttpRequest::ProcessTimeout() const {
  // std::cout << "处理请求超时" << std::endl;
  Proto::AccountCheckToHttpRs checkProto;
  checkProto.set_account(_account);
  checkProto.set_return_code(
      Proto::AccountCheckReturnCode::ARC_TIMEOUT); // 设置超时返回码

  MessageSystemHelp::DispatchPacket(Proto::MsgId::MI_AccountCheckToHttpRs,0,checkProto); // 分发数据包
}

// 处理请求结束
bool HttpRequest::ProcessOver() {
  curl_multi_remove_handle(_pMultiHandle,
                           _pCurl);  // 从多句柄管理器中移除CURL句柄
  curl_easy_cleanup(_pCurl);         // 清理CURL句柄
  curl_multi_cleanup(_pMultiHandle); // 清理多句柄管理器

  _pCurl = nullptr;
  _pMultiHandle = nullptr;
  return true;
}

// 处理请求
bool HttpRequest::Process() {
  int running_handle_count; // 代表了仍在处理的 HTTP 请求的数量
  CURLMcode curlMcode = curl_multi_perform(
      _pMultiHandle, &running_handle_count); // 执行CURL多句柄操作

  // std::cout << "curlMcode: " << curlMcode
  //           << " running_handle_count: " << running_handle_count << std::endl;

  if (CURLM_CALL_MULTI_PERFORM == curlMcode)
    return false; // 如果需要再次调用curl_multi_perform，返回false

  if (curlMcode != CURLMcode::CURLM_OK) {
    _curlRs = CRS_CURLMError; // 处理错误
    std::cout << _curlRs << std::endl;
    State = HRS_Timeout; // 更新状态为超时
    return false;
  }

  if (running_handle_count == 0) {
    ProcessMsg(); // 处理消息
    return true;
  }
  CURLMRS rs = curl_multi_select(_pMultiHandle); // 执行选择操作
  if (rs != CRS_OK && rs != CRS_CURLM_CALL_MULTI_PERFORM) {
    _curlRs = rs;
    State = HRS_Timeout; // 更新状态为超时
    return false;
  }

  return false;
}

// 处理消息
void HttpRequest::ProcessMsg() {
  // std::cout << "处理消息 " << std::endl;
  int msgs_left;
  CURLMsg *msg =
      curl_multi_info_read(_pMultiHandle, &msgs_left); // 读取多句柄信息

  if (msg == nullptr) {
    std::cerr << "curl_multi_info_read returned nullptr" << std::endl;
    return;
  }

  if (msg->msg == CURLMSG_DONE) {
    // 检查CURL请求的结果
    if (msg->data.result != CURLE_OK) {
      std::cerr << "Curl request failed: "
                << curl_easy_strerror(msg->data.result) << std::endl;
      ProcessTimeout(); // 处理超时情况
      return;
    }

    // 请求成功，输出响应内容
    // std::cout << "Curl request succeeded. Response: " << _responseBuffer
    //           << std::endl;

    // 解析JSON响应
    Json::CharReaderBuilder readerBuilder;
    std::unique_ptr<Json::CharReader> jsonReader(readerBuilder.newCharReader());
    Json::Value value;
    JSONCPP_STRING errs;
    bool ok = jsonReader->parse(_responseBuffer.data(),
                                _responseBuffer.data() + _responseBuffer.size(),
                                &value, &errs);

    if (ok && errs.empty()) {
      ProcessMsg(value); // 处理JSON消息，调用纯虚函数，由子类实现
    } else {
      std::cerr << "JSON parse failed. Errors: " << errs
                << " Response: " << _responseBuffer << std::endl;
    }
  } else {
    std::cerr << "ProcessMsg: Unhandled CURLMsg type: " << msg->msg
              << std::endl;
  }
}
