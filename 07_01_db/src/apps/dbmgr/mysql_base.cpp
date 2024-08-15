#include "mysql_base.h"
#include "libserver/log4_help.h"
#include <cstdlib>
#include <mysql/mysql.h>

// 初始化 MySQL 连接
bool MysqlBase::ConnectInit() {
  // 断开当前的 MySQL 连接（如果有的话）
  Disconnect();

  // 初始化 MySQL 对象
  _pMysql = mysql_init(nullptr);
  if (_pMysql == nullptr) {
    // 如果初始化失败，记录错误并返回 false
    CheckMysqlError();
    LOG_ERROR("mysql_init == nullptr");
    return false;
  }

  // 设置连接超时时间为 10 秒
  int outtime = 10;
  mysql_options(_pMysql, MYSQL_OPT_CONNECT_TIMEOUT, &outtime);

  // 设置自动重连选项
  bool reConnect = false;
  mysql_options(_pMysql, MYSQL_OPT_RECONNECT, &reConnect);

  // 初始化字段数和结果集指针
  _numFields = 0;
  _pMysqlFields = nullptr;
  _pMysqlRes = nullptr;
  return true;
}

// 断开 MySQL 连接并清理资源
void MysqlBase::Disconnect() {
  _pMysqlFields = nullptr;
  if (_pMysqlRes != nullptr) {
    mysql_free_result(_pMysqlRes);
    _pMysqlRes = nullptr;
  }

  if (_pMysql != nullptr) {
    mysql_close(_pMysql);
    _pMysql = nullptr;
  }
}

// 检查 MySQL 错误并记录错误信息
int MysqlBase::CheckMysqlError() const {
  const int mysqlerrno = mysql_errno(_pMysql);
  if (mysqlerrno > 0) {
    LOG_ERROR("MysqlConnector::CheckError. mysql_error = " << mysqlerrno);
    return mysqlerrno;
  }
  return 0;
}

// 执行 SQL 查询，并返回受影响的行数
bool MysqlBase::Query(const char *sql, my_ulonglong &affected_rows) {
  // 如果之前有结果集，释放它
  if (nullptr != _pMysqlRes) {
    mysql_free_result(_pMysqlRes);
    _pMysqlRes = nullptr;
  }

  // 执行 SQL 查询
  if (mysql_query(_pMysql, sql) != 0) {
    LOG_ERROR("Query error: " << mysql_errno(_pMysql) << " sql: " << sql);
    return false;
  }

  // 存储结果集
  _pMysqlRes = mysql_store_result(_pMysql);
  if (_pMysqlRes != nullptr) {
    _numFields = mysql_num_fields(_pMysqlRes);
    _pMysqlFields = mysql_fetch_fields(_pMysqlRes);
  }
  affected_rows = mysql_affected_rows(_pMysql);
  return true;
}

// 从结果集中获取一行数据
MYSQL_ROW MysqlBase::Fetch() const {
  if (_pMysqlRes == nullptr) {
    return nullptr;
  }
  return mysql_fetch_row(_pMysqlRes);
}

// 从结果行中获取整数值
int MysqlBase::GetInt(MYSQL_ROW row, int index) {
  if (row[index] == nullptr) {
    LOG_ERROR("!!! Failed. MysqlConnector::GetInt");
    return 0;
  }
  return atoi(row[index]);
}

// 从结果行中获取无符号整数值
unsigned int MysqlBase::GetUint(MYSQL_ROW row, int index) {
  if (row[index] == nullptr) {
    LOG_ERROR("!!! Failed. MysqlConnector::GetUint");
    return 0;
  }
  return strtoul(row[index], nullptr, 10);
}

// 从结果行中获取 64 位无符号整数值
uint64 MysqlBase::GetUint64(MYSQL_ROW row, int index) {
  if (row[index] == nullptr) {
    LOG_ERROR("!!! Failed. MysqlConnector::GetUint64");
    return 0;
  }
  return atoll(row[index]);
}

// 从结果行中获取字符串
char *MysqlBase::GetString(MYSQL_ROW row, int index) {
  if (row[index] == nullptr) {
    LOG_ERROR("!!! Failed. MysqlConnector::GetString");
    return nullptr;
  }
  return row[index];
}

// 从结果行中获取 BLOB 数据
int MysqlBase::GetBlob(MYSQL_ROW row, int index, char *buf, unsigned long size) const {
  unsigned int l = size > 0 ? size : 0;
  if (row[index] == nullptr) {
    LOG_ERROR("!!! Failed. MysqlConnector::GetBlob");
    return 0;
  }

  // 获取 BLOB 数据的长度
  unsigned long *lengths = mysql_fetch_lengths(_pMysqlRes);
  if (lengths[index] < l) {
    l = lengths[index];
  }
  memcpy(buf, row[index], l);
  return l;
}

// 从结果行中获取 BLOB 数据并存储到 std::string 中
void MysqlBase::GetBlob(MYSQL_ROW row, int index, std::string &protoStr) const {
  unsigned long *pLengths = mysql_fetch_lengths(_pMysqlRes);
  long blobLength = pLengths[index];

  if (blobLength <= 0) {
    protoStr = "";
    return;
  }

  // 分配内存并获取 BLOB 数据
  char *blobByte = new char[blobLength + 1];
  GetBlob(row, index, blobByte, blobLength);
  blobByte[blobLength] = '\0';

  // 将 BLOB 数据存储到 std::string 中
  protoStr = blobByte;
  delete[] blobByte;
}
