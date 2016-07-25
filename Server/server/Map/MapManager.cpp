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

void MapManager::Update(sf::Time p_Diff)
{
    for (std::map<uint16, Map*>::iterator l_It = m_MapList.begin(); l_It != m_MapList.end(); ++l_It)
    {
        Map* l_Map = (*l_It).second;
        if (l_Map == nullptr)
            continue;

        l_Map->Update(p_Diff);
    }
}
