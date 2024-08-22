#pragma once
#include "system.h"


class ResPath : public Component<ResPath>,public IAwakeSystem<>{
public:
    void Awake() override;
    void BackToPool() override;

    std::string FindResPath(const std::string& res);
    std::string FindResPath(const char* res);
private:
    std::string _resPath{""};
};