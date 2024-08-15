#include "mysql_connector.h"
#include "libserver/app_type_mgr.h"
#include "libserver/common.h"
#include "libserver/log4_help.h"
#include "libserver/yaml.h"
#include "mysql_base.h"
#include <cstring>
#include <log4cplus/tstring.h>
#include <mysql/field_types.h>
#include <mysql/mysql.h>
#include <mysql/mysql_com.h>
#include <thread>
#include <utility>

// 从连接池中唤醒并初始化 MySQL 连接配置
void MysqlConnector::AwakeFromPool() {
  // 记录当前线程的 ID 及唤醒日志
  LOG_DEBUG("MysqlConnector::Awake. id:" << std::this_thread::get_id());

  // 获取 YAML 配置实例
  auto pYaml = Yaml::GetInstance();
  // 从配置中获取数据库管理器的配置
  auto pConfig = pYaml->GetConfig(APP_DB_MGR);
  // 将配置转换为具体的数据库管理配置
  auto pDbConfig = dynamic_cast<DBMgrConfig *>(pConfig);

  // 获取 MySQL 数据库配置
  _pDbCfg = pDbConfig->GetDBConfig(DBMgrConfig::DBTypeMysql);
  if (_pDbCfg == nullptr) {
    // 如果无法获取 MySQL 配置，记录错误日志并返回
    LOG_ERROR("Failed to get mysql config.");
    return;
  }

  // 尝试连接 MySQL 数据库
  Connect();
}

// 检查 MySQL 连接的状态，必要时重新连接
void MysqlConnector::CheckPing() {
  if (_isRunning) {
    // 发送 MySQL ping 以检查连接是否存活
    mysql_ping(_pMysql);
    if (CheckMysqlError() != 0) {
      // 如果检测到错误，断开连接
      Disconnect();
    }
  }
  if (_isRunning) {
    // 如果连接仍然在运行，尝试重新连接
    ReConnect();
  }
}

// 将连接返回池中时断开 MySQL 连接
void MysqlConnector::BackToPool() {
  // 调用 Disconnect 方法断开 MySQL 连接
  Disconnect();
}

// 初始化并连接到 MySQL 数据库
bool MysqlConnector::Connect() {
  // 初始化连接
  if (!ConnectInit())
    return false;
  LOG_DEBUG("MysqlConnector::Connect. "
            << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port
            << "starting... id :" << std::this_thread::get_id());

  // 尝试连接到 MySQL 数据库
  mysql_real_connect(_pMysql, _pDbCfg->Ip.c_str(), _pDbCfg->User.c_str(),
                     _pDbCfg->Password.c_str(), _pDbCfg->DatabaseName.c_str(),
                     _pDbCfg->Port, nullptr, CLIENT_FOUND_ROWS);

  // 检查是否有连接错误
  int mysqlerrno = CheckMysqlError();
  if (mysqlerrno > 0) {
    // 如果连接失败，断开连接并返回 false
    Disconnect();
    return false;
  }

  // 再次检查连接是否存活
  mysql_ping(_pMysql);
  mysqlerrno = CheckMysqlError();
  if (mysqlerrno > 0) {
    // 如果 ping 检查失败，断开连接并返回 false
    Disconnect();
    return false;
  }

  LOG_DEBUG("MysqlConnector::Connect: successfully! addr: "
            << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port);

  // 初始化预处理语句
  InitStmts();

  // 标记连接为运行状态
  _isRunning = true;

  return true;
}

// 重新连接到 MySQL 数据库
void MysqlConnector::ReConnect() {
  // 初始化连接
  if (!ConnectInit()) {
    return;
  }
  LOG_DEBUG("MysqlConnector::ReConnect. " << _pDbCfg->Ip.c_str() << ":"
                                          << _pDbCfg->Port << "starting...");

  // 重新尝试连接
  if (nullptr == mysql_real_connect(
                     _pMysql, _pDbCfg->Ip.c_str(), _pDbCfg->User.c_str(),
                     _pDbCfg->Password.c_str(), _pDbCfg->DatabaseName.c_str(),
                     _pDbCfg->Port, nullptr, CLIENT_FOUND_ROWS)) {
    // 如果连接失败，记录错误日志
    LOG_ERROR("MysqlConnector::ReConnect failed. " << _pDbCfg->Ip.c_str() << ":"
                                                   << _pDbCfg->Port);
    return;
  }

  // 选择数据库
  mysql_select_db(_pMysql, _pDbCfg->DatabaseName.c_str());

  // 检查是否有错误
  if (CheckMysqlError() != 0) {
    Disconnect();
    return;
  }

  LOG_DEBUG("MysqlConnector::ReConnect: successfully! addr:"
            << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port);

  // 初始化预处理语句
  InitStmts();

  // 标记连接为运行状态
  _isRunning = true;
}

// 断开与 MySQL 数据库的连接并清理资源
void MysqlConnector::Disconnect() {
  // 清理预处理语句
  CleanStmts();
  // 调用基类的 Disconnect 方法关闭连接
  MysqlBase::Disconnect();
}

// 初始化常用的 SQL 预处理语句
void MysqlConnector::InitStmts() {
  // 创建插入语句的预处理
  DatabaseStmt *stmt =
      CreateStmt("insert into player ( sn, account, name, savetime, "
                 "createtime) value ( ?, ? ,? , now(), now())");

  // 将创建的语句插入到语句映射表中
  _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtCreate, stmt));

  // 创建更新语句的预处理
  stmt = CreateStmt(
      "update player set base=?, misc= ?,savetime=now() where sn = ?");

  // 将创建的语句插入到语句映射表中
  _mapStmt.insert(std::make_pair(DatabaseStmtKey::StmtSava, stmt));

  LOG_DEBUG("\tMysqlConnector::InitStmts successfully!");
}

// 清理所有的 SQL 预处理语句
void MysqlConnector::CleanStmts() {
  // 遍历语句映射表，并关闭所有语句
  for (auto one : _mapStmt) {
    one.second->Close();
  }
  // 清空语句映射表
  _mapStmt.clear();
}

// 根据关键字获取对应的 SQL 预处理语句
DatabaseStmt *MysqlConnector::GetStmt(DatabaseStmtKey stmtKey) {
  const auto iter = _mapStmt.find(stmtKey);
  if (iter == _mapStmt.end()) {
    // 如果没有找到对应的语句，返回 nullptr
    return nullptr;
  }
  return iter->second;
}

// 创建一个 SQL 预处理语句
DatabaseStmt *MysqlConnector::CreateStmt(const char *sql) const {
  int str_len = strlen(sql);
  DatabaseStmt *stmt = new DatabaseStmt;
  int param_count = 0;
  stmt->stmt = mysql_stmt_init(_pMysql);
  // 准备 SQL 语句
  if (mysql_stmt_prepare(stmt->stmt, sql, str_len) != 0) {
    // 如果准备失败，记录错误日志并返回 nullptr
    LOG_ERROR("create_stmt error, "
              << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(stmt->stmt)));
    return nullptr;
  }

  // 统计 SQL 语句中的参数个数
  for (int i = 0; i < str_len; i++) {
    if ((sql[i] == '?' || (sql[i] == '@')))
      param_count++;
  }

  // 如果有参数，则分配相应的内存空间
  if (param_count > 0) {
    stmt->bind = new MYSQL_BIND[param_count];
    memset(stmt->bind, 0, sizeof(MYSQL_BIND) * param_count);
    stmt->bind_buffer = new char[MAX_BIND_BUFFER];
  } else {
    stmt->bind = nullptr;
    stmt->bind_buffer = nullptr;
  }
  return stmt;
}

// 清空预处理语句中的参数
void MysqlConnector::ClearStmtParam(DatabaseStmt *stmt) {
  stmt->bind_index = 0;
  stmt->bind_buffer_index = 0;
}

// 执行预处理语句并获取受影响的行数
bool MysqlConnector::ExecuteStmt(DatabaseStmt *stmt,
                                 my_ulonglong &affected_rows) {
  MYSQL_STMT *st = stmt->stmt;
  if (NULL != stmt->bind)
    mysql_stmt_bind_param(stmt->stmt, stmt->bind);
  // 执行预处理语句
  if (mysql_stmt_execute(st) != 0) {
    // 如果执行失败，记录错误日志
    LOG_ERROR("execute_stmt error, "
              << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
    return false;
  }

  // 获取受影响的行数
  affected_rows = mysql_stmt_affected_rows(st);
  return true;
}

// 执行预处理语句
bool MysqlConnector::ExecuteStmt(DatabaseStmt *stmt) {
  MYSQL_STMT *st = stmt->stmt;
  if (NULL != stmt->bind)
    mysql_stmt_bind_param(stmt->stmt, stmt->bind);
  // 执行预处理语句
  if (mysql_stmt_execute(st) != 0) {
    // 如果执行失败，记录错误日志
    LOG_ERROR("execute_stmt error, "
              << LOG4CPLUS_C_STR_TO_TSTRING(mysql_stmt_error(st)));
    return false;
  }
  return true;
}

// 添加BLOB类型的参数到预处理语句中
void MysqlConnector::AddParamBlob(DatabaseStmt *stmt, void *val, int size) {
  // 获取当前绑定的参数
  MYSQL_BIND *pBind = &stmt->bind[stmt->bind_index];

  // 设置参数类型为 BLOB
  pBind->buffer_type = MYSQL_TYPE_BLOB;

  // 将输入的值赋给绑定缓冲区
  pBind->buffer = val;

  // 计算并设置参数的长度
  pBind->length = reinterpret_cast<unsigned long *>(
      &stmt->bind_buffer[stmt->bind_buffer_index + sizeof(void *)]);
  *(pBind->length) = size;

  // 设置缓冲区长度
  pBind->buffer_length = size;

  // 更新绑定索引和缓冲区索引
  stmt->bind_index++;
  stmt->bind_buffer_index += (sizeof(void *) + sizeof(unsigned long *));
}

// 添加无符号64位整数参数到预处理语句中
void MysqlConnector::AddParamUnit64(DatabaseStmt *stmt, uint64 val) {
  // 获取当前绑定的参数
  MYSQL_BIND *pBind = &stmt->bind[stmt->bind_index];

  // 设置参数类型为 LONGLONG (64位整数)
  pBind->buffer_type = MYSQL_TYPE_LONGLONG;

  // 设置为无符号整数
  pBind->is_unsigned = true;

  // 将输入的值赋给绑定缓冲区
  pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index];
  *static_cast<uint64 *>(pBind->buffer) = val;

  // 更新绑定索引和缓冲区索引
  stmt->bind_index++;
  stmt->bind_buffer_index += sizeof(uint64);
}

// 添加整数参数到预处理语句中
void MysqlConnector::AddParamInt(DatabaseStmt *stmt, int val) {
  // 获取当前绑定的参数
  MYSQL_BIND *pBind = &stmt->bind[stmt->bind_index];

  // 设置参数类型为 LONG (整数)
  pBind->buffer_type = MYSQL_TYPE_LONG;

  // 设置为有符号整数
  pBind->is_unsigned = false;

  // 将输入的值赋给绑定缓冲区
  pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index];
  *static_cast<long *>(pBind->buffer) = val;

  // 更新绑定索引和缓冲区索引
  stmt->bind_index++;
  stmt->bind_buffer_index += sizeof(long);
}

// 添加字符串参数到预处理语句中
void MysqlConnector::AddParamStr(DatabaseStmt *stmt, const char *value) {
  // 获取当前绑定的参数
  MYSQL_BIND *pBind = &stmt->bind[stmt->bind_index];

  // 获取字符串长度
  int len = strlen(value);

  // 检查字符串长度是否超出最大限制
  if (len >= MAX_BIND_STR) {
    LOG_ERROR("add_str_param error, str size: " << len << " out of limit: "
                                                << MAX_BIND_STR);
    len = MAX_BIND_STR - 1; // 截断字符串
  }

  // 设置参数类型为 STRING (字符串)
  pBind->buffer_type = MYSQL_TYPE_STRING;

  // 将字符串复制到绑定缓冲区
  pBind->buffer = &stmt->bind_buffer[stmt->bind_buffer_index];
  engine_strncpy((char *)pBind->buffer, len + 1, value, len + 1);

  // 计算并设置参数的长度
  pBind->length =
      (unsigned long *)&stmt->bind_buffer[stmt->bind_buffer_index + len + 1];
  *(pBind->length) = len;

  // 设置缓冲区长度
  pBind->buffer_length = len;

  // 更新绑定索引和缓冲区索引
  stmt->bind_index++;
  stmt->bind_buffer_index += (len + 1 + sizeof(unsigned long *));
}