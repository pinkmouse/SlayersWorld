#include "MapManager.hpp"
#define NB_MAP 2

MapManager::MapManager(Events* p_Events) :
    m_Events(p_Events),
	m_ActualMap(nullptr)
{
    m_PosX = 0;
    m_PosY = 0;
    m_MainPlayer = nullptr;
}


MapManager::~MapManager()
{
}

void MapManager::Update(sf::Time p_Diff)
{
    if (m_ActualMap != nullptr)
        m_ActualMap->Update(p_Diff);
}

bool MapManager::LoadMap(uint16 p_MapID, const std::string & p_MapFileName, const std::string & p_MapChispetName, const std::string & p_MapName)
{
	m_ActualMap = new Map(p_MapID);
    if (!m_ActualMap->InitializeMap(p_MapFileName, p_MapChispetName))
        return false;
    return true;
}

Map* MapManager::GetActualMap() const
{
	return m_ActualMap;
}

void MapManager::InitializeMaps()
{
    /*for (uint8 i = 0; i < NB_MAP; ++i)
	    m_MapsPath[i] = "map" + std::to_string(i) + ".map";*/
}

bool  MapManager::HasMap()
{
	if (m_ActualMap == nullptr)
		return false;

	if (!m_ActualMap->IsValidMap())
		return false;

	return true;
}

void MapManager::SetPosX(uint32 p_PosX)
{
    m_PosX = p_PosX;
}

void MapManager::SetPosY(uint32 p_PosY)
{
    m_PosY = p_PosY;
}

uint32 MapManager::GetPosX() const
{
    return m_PosX;
}

uint32 MapManager::GetPosY() const
{
    return m_PosY;

}