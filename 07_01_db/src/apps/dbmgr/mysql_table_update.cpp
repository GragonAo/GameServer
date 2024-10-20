#include "mysql_table_update.h"
#include "libserver/common.h"
#include "libserver/log4_help.h"
#include "libserver/util_string.h"
#include "libserver/yaml.h"
#include <log4cplus/tstring.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#include <thread>

// 构造函数：初始化更新函数列表，添加一个空更新函数
MysqlTableUpdate::MysqlTableUpdate() {
  _update_func.push_back(BindFunP0(this, &MysqlTableUpdate::Update00));
}

// 核心检查函数，用于检查数据库连接和执行必要的更新操作
void MysqlTableUpdate::Check() {
  // 初始化数据库连接
  if (!ConnectInit()) {
    return;
  }

  // 获取数据库配置
  auto pYaml = Yaml::GetInstance();
  auto pDbMgrConfig = dynamic_cast<DBMgrConfig *>(pYaml->GetConfig(APP_DB_MGR));
  _pDbCfg = pDbMgrConfig->GetDBConfig(DBMgrConfig::DBTypeMysql);
  
  if (_pDbCfg == nullptr) {
    LOG_ERROR("Init failed. get mysql config is failed.");
    return;
  }
  
  LOG_DEBUG("Mysql udpate connect. "
            << _pDbCfg->Ip.c_str() << ":" << _pDbCfg->Port
            << " starting...id: " << std::this_thread::get_id());

  // 连接到 MySQL 数据库
  if (mysql_real_connect(_pMysql, _pDbCfg->Ip.c_str(),
                         _pDbCfg->User.c_str(),
                         _pDbCfg->Password.c_str(), nullptr,
                         _pDbCfg->Port, nullptr, CLIENT_FOUND_ROWS)) {
    mysql_select_db(_pMysql, _pDbCfg->DatabaseName.c_str());
  }

  // 检查是否有 MySQL 错误
  int mysqlerrno = CheckMysqlError();
  if (mysqlerrno == ER_BAD_DB_ERROR) {
    LOG_DEBUG("Mysql. try create database: " << _pDbCfg->DatabaseName.c_str());

    // 如果数据库不存在，尝试创建数据库
    if (!CreateDatabaseIfNotExist()) {
      Disconnect();
      return;
    }
    mysql_select_db(_pMysql, _pDbCfg->DatabaseName.c_str());
    mysqlerrno = CheckMysqlError();
  }

  // 如果仍有错误，断开连接
  if (mysqlerrno > 0) {
    Disconnect();
    return;
  }

  // 更新数据库到最新版本
  if (!UpdateToVersion()) {
    LOG_ERROR("!!!Failed. Mysql update. UpdateToVersion");
    return;
  }

  // 发送 MySQL ping 以保持连接
  mysql_ping(_pMysql);
  mysqlerrno = CheckMysqlError();
  if (mysqlerrno > 0) {
    Disconnect();
    return;
  }
  
  LOG_DEBUG("Mysql Update successully! addr: " << _pDbCfg->Ip.c_str() << ":"
                                               << _pDbCfg->Port);
}

// 如果数据库不存在，创建数据库并进行基本设置
bool MysqlTableUpdate::CreateDatabaseIfNotExist() {
  // 构建创建数据库的 SQL 语句
  std::string querycmd = strutil::format("CREATE DATABASE IF NOT EXISTS %s;",
                                         _pDbCfg->DatabaseName.c_str());

  my_ulonglong affected_rows;
  if (!Query(querycmd.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateDatabaseIfNotExit. cmd:"
              << querycmd.c_str());
    return false;
  }

  // 选择数据库
  if (mysql_select_db(_pMysql, _pDbCfg->DatabaseName.c_str()) != 0) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateDatabaseIfNotExist: "
              "mysql_select_db :"
              << LOG4CPLUS_TSTRING_TO_STRING(_pDbCfg->DatabaseName))
    return false;
  }

  // 设置字符集
  if (mysql_set_character_set(_pMysql, _pDbCfg->CharacterSet.c_str()) != 0) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateDatabaseIfNotExist: Could "
              "not set client connection character set to"
              << LOG4CPLUS_STRING_TO_TSTRING(_pDbCfg->CharacterSet));
    return false;
  }

  // 更改数据库的字符集和排序规则
  querycmd = strutil::format("ALTER DATABASE CHARACTER SET %s COLLATE %s",
                             _pDbCfg->CharacterSet.c_str(),
                             _pDbCfg->Collation.c_str());
  if (!Query(querycmd.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateDatabaseIfNotExist. cmd:"
              << LOG4CPLUS_STRING_TO_TSTRING(querycmd.c_str()));
    return false;
  }

  // 创建版本表
  std::string create_version = "CREATE TABLE IF NOT EXISTS `version` ("
                               "`version` int(11) NOT NULL,"
                               "PRIMARY KEY (`version`)"
                               ") ENGINE=%s DEFAULT CHARSET=%s;";

  std::string cmd = strutil::format(create_version.c_str(), "InnoDB",
                                    _pDbCfg->CharacterSet.c_str());
  if (!Query(cmd.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateTable. "
              << LOG4CPLUS_STRING_TO_TSTRING(cmd));
    return false;
  }

  // 创建玩家表
  std::string create_player = "CREATE TABLE IF NOT EXISTS `player` ("
                              "`sn` bigint(20) NOT NULL,"
                              "`name` char(32) NOT NULL,"
                              "`account` char(64) NOT NULL,"
                              "`base` blob,"
                              "`item` blob,"
                              "`misc` blob,"
                              "`savetime` datetime default NULL,"
                              "`createtime` datetime default NULL,"
                              "PRIMARY KEY  (`sn`),"
                              "UNIQUE KEY `NAME` (`name`),"
                              "KEY `ACCOUNT` (`account`)"
                              ") ENGINE=%s DEFAULT CHARSET=%s;";

  cmd = strutil::format(create_player.c_str(), "InnoDB",
                        _pDbCfg->CharacterSet.c_str());
  if (!Query(cmd.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateTable"
              << LOG4CPLUS_STRING_TO_TSTRING(cmd));
    return false;
  }

  // 插入初始版本号
  cmd = "insert into `version` VALUES ('0')";
  if (!Query(cmd.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::CreateTable."
              << LOG4CPLUS_STRING_TO_TSTRING(cmd));
    return false;
  }

  return true;
}

// 更新数据库到最新版本
bool MysqlTableUpdate::UpdateToVersion() {
  my_ulonglong affected_rows;
  
  // 查询当前版本号
  std::string sql = "select version from `version`";
  if (!Query(sql.c_str(), affected_rows)) {
    return false;
  }

  // 获取查询结果
  MYSQL_ROW row = Fetch();
  if (row == nullptr) {
    return false;
  }

  // 获取数据库当前版本
  int version = GetInt(row, 0);
  
  // 如果数据库已经是最新版本，直接返回
  if (version == _version) {
    return true;
  }

  // 按顺序更新到最新版本
  for (int i = version + 1; i <= _version; i++) {
    if (_update_func[i] == nullptr)
      continue;

    // 执行更新函数
    if (!_update_func[i]()) {
      LOG_ERROR("UpdateToVersion failed!!!! version = " << i);
      return false;
    }

    // 更新成功，记录新版本号
    LOG_INFO("update db to version:" << i);
    std::string cmd = strutil::format("update `version` set version = %d", i);
    if (!Query(cmd.c_str(), affected_rows)) {
      LOG_ERROR(
          "UpdateToVersion failed!!! change version failed. version = " << i);
      return false;
    }
  }

  return true;
}

// 初始版本更新函数（空函数）
bool MysqlTableUpdate::Update00() { return true; }
