#include "MapManager.hpp"



MapManager::MapManager() :
	m_ActualMap(nullptr)
{
}


MapManager::~MapManager()
{
}

void MapManager::LoadMap(uint8 m_MapID)
{
	m_ActualMap = new Map();
	m_ActualMap->InitializeMap(m_MapsPath[0]);
}

Map* MapManager::GetActualMap() const
{
	return m_ActualMap;
}

void MapManager::InitializeMaps()
{
	m_MapsPath[0] = "map00.map";
}

bool  MapManager::HasMap()
{
	if (m_ActualMap == nullptr)
		return false;

	if (!m_ActualMap->IsValidMap())
		return false;

	return true;
}
