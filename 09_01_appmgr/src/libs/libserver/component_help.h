#pragma once

#include "entity_system.h"
#include "res_path.h"
#include "yaml.h"
class ComponentHelp {
public:
  static EntitySystem *GetGlobalEntitySystem();
  static Yaml *GetYaml();
  static ResPath *GetResPath();
};