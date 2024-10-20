#include "libserver/common.h"
#include "libserver/log4_help.h"
#include "libserver/message_list.h"
#include "libserver/protobuf/db.pb.h"
#include "libserver/protobuf/msg.pb.h"
#include "libserver/protobuf/proto_id.pb.h"
#include "libserver/util_string.h"
#include "mysql_connector.h"
#include <mysql/mysql.h>

// 注册消息处理函数
void MysqlConnector::RegisterMsgFunction() {
  auto pMsgCallBack = new MessageCallBackFunction();
  AttachCallbackHandler(pMsgCallBack); // 关联回调处理程序

  // 注册处理玩家列表查询的回调函数
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::L2DB_QueryPlayerList,
      BindFunP1(this, &MysqlConnector::HandleQueryPlayerList));

  // 注册处理创建玩家的回调函数
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::L2DB_CreatePlayer,
      BindFunP1(this, &MysqlConnector::HandleCreatePlayer));

  // 注册处理保存玩家的回调函数
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::G2DB_SavePlayer,
      BindFunP1(this, &MysqlConnector::HandleSavePlayer));

  // 注册处理玩家查询的回调函数
  pMsgCallBack->RegisterFunction(
      Proto::MsgId::G2DB_QueryPlayer,
      BindFunP1(this, &MysqlConnector::HandlQueryPlayer));
}

// 处理玩家列表查询的函数
void MysqlConnector::HandleQueryPlayerList(Packet *pPacket) {
  // 解析收到的消息
  auto protoQuery = pPacket->ParseToProto<Proto::QueryPlayerList>();
  // 调用查询玩家列表的函数
  QueryPlayerList(protoQuery.account(), pPacket->GetSocket());
}

// 查询玩家列表的具体实现
void MysqlConnector::QueryPlayerList(std::string account, SOCKET socket) {
  my_ulonglong affected_rows;
  // 构建SQL查询语句
  std::string sql = strutil::format(
      "select sn, name, base, item, misc from player where account = '%s'",
      account.c_str());

  // 执行SQL查询
  if (!Query(sql.c_str(), affected_rows)) {
    LOG_ERROR("!!! Failed. MysqlConnector::HandleQueryPlayerList. sql:"
              << sql.c_str());
    return;
  }

  Proto::PlayerList protoRs; // 用于存储查询结果的Proto对象
  protoRs.set_account(account.c_str());

  Proto::PlayerBase protoBase;

  if (affected_rows > 0) {
    std::string tempStr;
    MYSQL_ROW row;
    // 逐行处理查询结果
    while ((row = Fetch())) {
      auto pProtoPlayer = protoRs.add_player();
      pProtoPlayer->set_sn(GetUint64(row, 0)); // 设置玩家序列号
      pProtoPlayer->set_name(GetString(row, 1)); // 设置玩家名称

      GetBlob(row, 2, tempStr); // 获取玩家基础信息
      protoBase.ParseFromString(tempStr);
      pProtoPlayer->set_level(protoBase.level()); // 设置玩家等级
      pProtoPlayer->set_gender(protoBase.gender()); // 设置玩家性别
    }
  }
  LOG_DEBUG("player list. account: " << account.c_str() << " player list size: "
                                     << protoRs.player_size()
                                     << " socket:" << socket);

  // 即使没有找到结果，也需要返回响应包
  SendPacket(Proto::MsgId::L2DB_QueryPlayerListRs, socket, protoRs);
}

// 处理单个玩家查询的函数
void MysqlConnector::HandlQueryPlayer(Packet *pPacket) {
  auto protoQuery = pPacket->ParseToProto<Proto::QueryPlayer>();

  my_ulonglong affected_rows;

  // 构建查询指定玩家的SQL语句
  std::string sql = strutil::format(
      "select sn, name, account, base, item, misc from player where sn = %llu",
      protoQuery.player_sn());

  // 执行SQL查询
  if (!Query(sql.c_str(), affected_rows)) {
    LOG_ERROR(
        "!!! Failed. MysqlConnector::HandleQueryPlayer. sql: " << sql.c_str());
    return;
  }

  Proto::QueryPlayerRs protoRs; // 用于存储查询结果的Proto对象
  if (affected_rows > 0) {
    std::string tempStr;
    MYSQL_ROW row;
    if ((row = Fetch())) {
      Proto::Player *pProtoPlayer = protoRs.mutable_player();
      pProtoPlayer->set_sn(GetUint64(row, 0)); // 设置玩家序列号
      pProtoPlayer->set_name(GetString(row, 1)); // 设置玩家名称
      protoRs.set_account(GetString(row, 2)); // 设置玩家账户

      GetBlob(row, 3, tempStr); // 获取玩家基础信息
      pProtoPlayer->mutable_base()->ParseFromString(tempStr);

      GetBlob(row, 5, tempStr); // 获取玩家其他信息
      pProtoPlayer->mutable_misc()->ParseFromString(tempStr);
    }
  }

  // 发送查询结果的响应包
  SendPacket(Proto::MsgId::G2DB_QueryPlayerRs, pPacket->GetSocket(), protoRs);
}

// 处理创建玩家的函数
void MysqlConnector::HandleCreatePlayer(Packet *pPacket) {
  auto protoCreate = pPacket->ParseToProto<Proto::CreatePlayerToDB>();
  auto protoPlayer = protoCreate.player();

  // 获取创建玩家的预处理语句
  DatabaseStmt *stmt = GetStmt(DatabaseStmtKey::StmtCreate);
  if (stmt == nullptr) {
    return;
  }
  LOG_INFO("HandlePlayerCreate sn:" << protoPlayer.sn() << " account: "
                                    << protoCreate.account().c_str()
                                    << " name: " << protoPlayer.name().c_str());

  ClearStmtParam(stmt); // 清除之前的绑定参数
  AddParamUnit64(stmt, protoPlayer.sn()); // 绑定玩家序列号
  AddParamStr(stmt, protoCreate.account().c_str()); // 绑定账户
  AddParamStr(stmt, protoPlayer.name().c_str()); // 绑定玩家名称

  Proto::CreatePlayerToDBRs protoRs; // 创建玩家的响应对象
  protoRs.set_account(protoCreate.account().c_str());
  protoRs.set_return_code(Proto::CreatePlayerReturnCode::CPR_Unkonwn); // 设置默认返回码

  // 执行创建玩家的SQL语句
  if (ExecuteStmt(stmt)) {
    protoRs.set_return_code(Proto::CreatePlayerReturnCode::CPR_Create_OK);
  }

  // 根据执行结果，返回创建成功后的玩家列表或错误响应
  if (protoRs.return_code() == Proto::CreatePlayerReturnCode::CPR_Create_OK) {
    QueryPlayerList(protoCreate.account(), pPacket->GetSocket());
  } else {
    SendPacket(Proto::MsgId::L2DB_CreatePlayerRs, pPacket->GetSocket(),
               protoRs);
  }
}

// 处理保存玩家信息的函数
void MysqlConnector::HandleSavePlayer(Packet *pPakcet) {
  auto proto = pPakcet->ParseToProto<Proto::SavePlayer>();
  DatabaseStmt *stmt = GetStmt(DatabaseStmtKey::StmtSava);

  if (stmt == nullptr)
    return;

  Proto::Player protoPlayer = proto.player();
  OnSavePlayer(stmt, protoPlayer); // 保存玩家信息
}

// 保存玩家信息的具体实现
bool MysqlConnector::OnSavePlayer(DatabaseStmt *stmtSave,
                                  Proto::Player &protoPlayer) {
  ClearStmtParam(stmtSave); // 清除之前的绑定参数

  std::string baseStr;
  protoPlayer.base().SerializeToString(&baseStr);
  AddParamBlob(stmtSave, (void *)baseStr.c_str(), (int)baseStr.size()); // 绑定玩家基础信息

  std::string miscStr;
  protoPlayer.misc().SerializeToString(&miscStr);
  AddParamBlob(stmtSave, (void *)miscStr.c_str(), (int)miscStr.size()); // 绑定玩家其他信息

  AddParamUnit64(stmtSave, protoPlayer.sn()); // 绑定玩家序列号

  // 执行保存操作，如果失败，记录错误日志
  if (!ExecuteStmt(stmtSave)) {
    LOG_ERROR("save player failed. player sn: " << protoPlayer.sn());
    return false;
  }
  return true;
}
