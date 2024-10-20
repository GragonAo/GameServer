#pragma once

enum RobotStateType {
  None = -1,

  Http_Connecting = 0,
  Http_Connected,

  Login_Connecting,
  Login_Connected,
  Login_Logined,

  Login_SelectPlayer,

  Game_Connecting,
  Game_Connected,
  Game_Logined,

  Space_EnterWorld,

  End,
};

const char RobotStateTypeName[][255] = {
    "[Http ] Connecting :",  "[Http ] Connected :", "[Login] Connecting :",
    "[Login] Connected :",   "[Login] Logined :",   "[Login] SelectPlayer:",
    "[Game ] Connecting :",  "[Game ] Connected :", "[Game ] Logined :",
    "[Space ] EnterWorld :",
};

const char RobotStateTypeShortName[][255] = {
    "Http-Connecting",  "Http-Connected", "Login-Connecting",
    "Login-Connected",  "Login-Logined",  "Login-SelectPlayer",

    "Game-Connecting",  "Game-Connected", "Game-Logined",

    "Space-EnterWorld",
};

inline const char *GetRobotStateTypeShortName(RobotStateType stateType) {
  if (stateType <= RobotStateType::None || stateType >= RobotStateType::End) {
    return "unknown";
  }
  return RobotStateTypeName[(int)stateType];
}

inline const char* GetRobotStateTypeName(RobotStateType stateType)
{
    if (stateType <= RobotStateType::None || stateType >= RobotStateType::End)
    {
        return "unknown";
    }

    return RobotStateTypeName[(int)stateType];
}