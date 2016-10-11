#include "LevelManager.hpp"
#include "../Global.hpp"

LevelManager::LevelManager()
{
}

LevelManager::~LevelManager()
{
}

bool LevelManager::Initialize()
{
    m_MapXpLevel = g_SqlManager->GetXpLevel();
    return true;
}

bool LevelManager::IsMaxLevel(uint8 p_Level)
{
    if (m_MapXpLevel[p_Level] <= 0)
        return true;
    return false;
}

float LevelManager::XpPct(uint8 p_Level, uint16 p_Xp)
{
    return ((float)p_Xp / ((float)GetXpForLevel(p_Level) / 100.0f));
}

uint16 LevelManager::GetXpForLevel(uint8 p_Level)
{
    return m_MapXpLevel[p_Level];
}