#include "MapManager.hpp"



MapManager::MapManager() :
	m_ActualMap(new Map())
{
}


MapManager::~MapManager()
{
}

void MapManager::LoadMap(uint8 m_MapID)
{
	m_ActualMap->InitializeMap(m_MapsPath[0]);
}


void MapManager::InitializeMaps()
{
	m_MapsPath[0] = "map00.map";
}