#include "sync_component.h"
#include "app_type.h"
#include "component_help.h"
#include "log4_help.h"
#include "packet.h"
#include <algorithm>

void AppInfo::Parse(Proto::AppInfoSync proto) {
  auto pYaml = ComponentHelp::GetYaml();
  this->AppId = proto.app_id();
  this->AppType = static_cast<APP_TYPE>(proto.app_type());
  this->Online = proto.online();

  auto pConfig = pYaml->GetIPEndPoint(this->AppType, this->AppId);
  this->Ip = pConfig->Ip;
  this->Port = pConfig->Port;
}

void SyncComponent::AppInfoSyncHandle(Packet *pPacket) {
  auto proto = pPacket->ParseToProto<Proto::AppInfoSync>();
  const auto iter = _apps.find(proto.app_id());
  if (iter == _apps.end()) {
    AppInfo syncAppInfo;
    syncAppInfo.Parse(proto);
    syncAppInfo.Socket = pPacket->GetSocket();
    _apps[syncAppInfo.AppId] = syncAppInfo;
  } else {
    const int appId = proto.app_id();
    _apps[appId].Online = proto.online();
    _apps[appId].Socket = pPacket->GetSocket();
  }
}

bool SyncComponent::GetOnApp(APP_TYPE appType, AppInfo &info) {
  if (_apps.size() == 0) {
    LOG_ERROR("GetApp failed. no more. appType: " << GetAppName(appType));
    return false;
  }

  auto iter = std::find_if(_apps.begin(), _apps.end(), [&appType](auto pair) {
    return (pair.second.AppType & appType) != 0;
  });

  if (iter == _apps.end()) {
    LOG_ERROR("GetApp failed. no more. appType: " << appType);
    return false;
  }

  auto min = iter->second.Online;
  int appId = iter->first;

  while (iter != _apps.end()) {
    if (min == 0)
      break;
    if ((iter->second.AppType & appType) == 0) {
      ++iter;
      continue;
    }

    if (iter->second.Online < min) {
      min = iter->second.Online;
      appId = iter->first;
    }
    ++iter;
  }

  auto syncInfo = _apps[appId];
  syncInfo.Online += 1;
  _apps[appId] = syncInfo;

  info = _apps[appId];
  return true;
}

void SyncComponent::CmdShow() {
  LOG_DEBUG("------------------------------------");
  for (auto pair : _apps) {
    LOG_DEBUG("appId: " << std::setw(4) << pair.first
                        << " type: " << GetAppName(pair.second.AppType)
                        << " online: " << pair.second.Online);
  }
}