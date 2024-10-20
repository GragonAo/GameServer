#pragma once

#include "libserver/singleton.h"
#include "mysql_base.h"
#include <functional>
#include <mysql/mysql.h>
#include <vector>

class MysqlTableUpdate : public MysqlBase, public Singleton<MysqlTableUpdate> {
public:
  MysqlTableUpdate();
  virtual ~MysqlTableUpdate() = default;
  void Check();

private:
  bool CreateDatabaseIfNotExist();
  bool UpdateToVersion();
  bool Update00();

private:
  typedef std::function<bool(void)> OnUpdate;
  std::vector<OnUpdate> _update_func;

  int const _version = 0;
};
