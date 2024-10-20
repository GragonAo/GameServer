#pragma once

#include "libserver/entity.h"
#include "libserver/message_system.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/socket_object.h"
#include "libserver/system.h"
#include "mysql_base.h"
#include <mysql/mysql.h>

class Packet;

enum DatabaseStmtKey {
  StmtCreate, // 插入
  StmtSava,   // 更新
};

struct DatabaseStmt {
  MYSQL_STMT *stmt{nullptr};
  MYSQL_BIND *bind{nullptr};
  char *bind_buffer{nullptr};
  int bind_index;
  int bind_buffer_index;

  void Close(){
    if (bind != nullptr) {
      delete[] bind;
      bind = nullptr;
    }
    if (bind_buffer != nullptr) {
      delete[] bind_buffer;
      bind_buffer = nullptr;
    }
    if (stmt != nullptr) {
      mysql_stmt_close(stmt);
      stmt = nullptr;
    }
  }
};

#define MAX_BIND_BUFFER 40960
#define MAX_BIND_STR 30000

class MysqlConnector : public MysqlBase,
                       public Entity<MysqlConnector>,
                       public IAwakeSystem<> {
public:
  void Awake() override;
  void BackToPool() override;
  void InitMessageComponent();

  bool Connect();
  void Disconnect() override;

private:
  void ReConnect();
  void CheckPing();

#pragma region
  void InitStmts();
  void CleanStmts();
  DatabaseStmt *GetStmt(DatabaseStmtKey stmtKey);
  DatabaseStmt *CreateStmt(const char *sql) const;

  void ClearStmtParam(DatabaseStmt *stmt);
  static void AddParamStr(DatabaseStmt *stmt, const char *value);
  static void AddParamInt(DatabaseStmt *stmt, int val);
  static void AddParamUnit64(DatabaseStmt *stmt, uint64 val);
  static void AddParamBlob(DatabaseStmt *stmt, void *val, int size);
  bool ExecuteStmt(DatabaseStmt *stmt, my_ulonglong &affected_rows);
  bool ExecuteStmt(DatabaseStmt *stmt);
#pragma endregion

  void HandleQueryPlayerList(Packet *pPacket);
  void QueryPlayerList(std::string account,NetworkIdentify* pIdentify);

  void HandlQueryPlayer(Packet *pPacket);
  void HandleCreatePlayer(Packet *pPacket);
  void HandleSavePlayer(Packet *pPakcet);
  bool OnSavePlayer(DatabaseStmt *stmtSave, Proto::Player &protoPlayer);

protected:
  std::map<DatabaseStmtKey, DatabaseStmt *> _mapStmt;
  bool _isRunning{false};
};