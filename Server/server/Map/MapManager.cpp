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

Map* MapManager::GetMap(uint16 p_Id)
{
    Map* l_Map = m_MapList[p_Id];
    return l_Map;
}

bool MapManager::IsOnline(TypeUnit p_TypeID, uint16 p_UnitID)
{
    for (std::pair<uint16, Map*> l_MapPair : m_MapList)
    {
        Map* l_Map = l_MapPair.second;

        if (l_Map == nullptr)
            continue;

        printf("Get Unit type %d id:%d\n", p_TypeID, p_UnitID);
        if (l_Map->GetUnit(p_TypeID, p_UnitID))
            return true;
    }
    return false;
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

Player* MapManager::GetPlayer(uint16 p_IdPlayer)
{
    for (std::pair<uint16, Map*> l_MapPair : m_MapList)
    {
        Map* l_Map = l_MapPair.second;

        if (l_Map == nullptr)
            continue;

        return l_Map->GetUnit(TypeUnit::PLAYER, p_IdPlayer)->ToPlayer();
    }
    return nullptr;
}