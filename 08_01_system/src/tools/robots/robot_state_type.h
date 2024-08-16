#pragma once

enum RobotStateType {
  RobotState_HttpRequest = 0,

  RobotState_Login_Connecting,
  RobotState_Login_Connected,
  RobotState_Login_Logined,
  RobotState_Login_SelectPlayer,

  RobotState_Game_Connecting,
  RobotState_Game_Connected,
  RobotState_Game_Logined,

  RobotState_Space_EnterWorld,

  RobotState_End,
};

const char RobotStateTypeName[][255] = {
    "[    ] HttpRequest :", "[Login] Connecting :",   "[Login] Connected :",
    "[Login] Logined :",    "[Login] SelectPlayer :", "[Game ] Connecting :",
    "[Game ] Connected :",  "[Game ] Logined :",      "[Space ] EnterWorld :",
};

const char RobotStateTypeShortName[][255] = {
    "HttpRequest",

    "Login-Connecting",   "Login-Connected", "Login-Logined",
    "Login-SelectPlayer",

    "Game-Connecting",    "Game-Connected",  "Game-Logined",

    "Space-EnterWorld",
};

inline const char *GetRobotStatetypeShortName(RobotStateType stateType) {
  if (stateType < 0 || stateType >= RobotState_End)
    return "unknown";
  return RobotStateTypeShortName[stateType];
}

inline const char *GetrobotStateTypeName(RobotStateType stateType) {
  if (stateType < 0 || stateType >= RobotState_End)
    return "unknown";
  return RobotStateTypeName[stateType];
}