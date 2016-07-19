#include <cstdio>
#include "MapManager.hpp"


MapManager::MapManager()
{
}


MapManager::~MapManager()
{
}

bool MapManager::InitializeMaps()
{
	Map* l_Map = new Map();
    if (!l_Map->InitializeMap("map00.map"))
        return false;
    else
        m_MapList[0] = l_Map;
	return true;
}

Map* MapManager::GetMap(uint16 p_Id) const
{
    Map* l_Map = m_MapList.at(p_Id);
    return l_Map;
}
