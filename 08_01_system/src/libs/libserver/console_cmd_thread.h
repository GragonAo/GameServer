#pragma once

#include "console.h"
#include <vector>

class ConsoleCmdThread : public ConsoleCmd {
public:
  void HandleHelp() override;
  void RegisterHandler() override;

private:
  void HandleEntity(std::vector<std::string> &params);
};