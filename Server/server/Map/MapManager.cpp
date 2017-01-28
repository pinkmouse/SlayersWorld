#include <cstdio>
#include "MapManager.hpp"
#include "../World/WorldSocket.hpp"
#define NB_MAP 2

MapManager::MapManager()
{
}


MapManager::~MapManager()
{
}

bool MapManager::InitializeMaps()
{
    for (uint16 i = 0; i < NB_MAP; ++i)
    {
        Map* l_Map = new Map();
        if (!l_Map->InitializeMap("map" + std::to_string(i) + ".map"))
            return false;
        else
            m_MapList[i] = l_Map;
    }
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
    /// Switch Unit of map
    for (std::map<uint16, Map*>::iterator l_It = m_MapList.begin(); l_It != m_MapList.end(); ++l_It)
    {
        Map* l_Map = (*l_It).second;
        if (l_Map == nullptr)
            continue;

        std::queue<Unit*>* l_UnitSwitchMapQueue = l_Map->GetUnitSwitchMapQueue();
        while (!l_UnitSwitchMapQueue->empty())
        {
            Unit* l_Unit = l_UnitSwitchMapQueue->front();
            Map* l_NewMap = GetMap(l_Unit->GetMapID());

            if (l_NewMap == nullptr || l_NewMap->GetID() == l_Map->GetID()) /// If new map doesn't exist, we don't switch it
            {
                l_Unit->SetMapID(l_Map->GetID());
                l_UnitSwitchMapQueue->pop();
                continue;
            }
            l_Map->RemoveUnit(l_Unit);
            l_NewMap->AddUnit(l_Unit);
            if (Player* l_Player = l_Unit->ToPlayer())
                l_Player->GetSession()->SendSwitchMap(l_NewMap->GetID());
            l_UnitSwitchMapQueue->pop();
        }
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