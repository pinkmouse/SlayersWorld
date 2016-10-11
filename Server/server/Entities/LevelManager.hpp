#pragma once

#include "../Define.hpp"
#include <map>

class LevelManager
{
public:
    LevelManager();
    ~LevelManager();
    bool Initialize();
    uint16 GetXpForLevel(uint8);
    bool IsMaxLevel(uint8);
    float XpPct(uint8, uint16);

private:
    std::map<uint8, uint16> m_MapXpLevel;
};
