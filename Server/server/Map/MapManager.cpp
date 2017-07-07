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

bool MapManager::InitializeMapsTemplate()
{
    for (auto l_MapTemplate : m_MapListTemplate)
    {
        if (!l_MapTemplate.second->InitializeMap())
            return false;
    }
    return true;
    /*for (uint16 i = 0; i < NB_MAP; ++i)
    {
        MapTemplate* l_Map = new MapTemplate(i);
        if (!l_Map->InitializeMap("map" + std::to_string(i) + ".map"))
            return false;
        else
            m_MapListTemplate[i] = l_Map;
    }
	return true;*/
}

Map* MapManager::LaunchMap(uint16 p_MapID)
{
    if (m_MapListTemplate[p_MapID] == nullptr)
        return nullptr;

    Map* l_Map = new Map(m_MapListTemplate[p_MapID]);
    return l_Map;
}

bool MapManager::LaunchWorldsMap()
{
    for (auto l_MapTemplate : m_MapListTemplate)
    {
        Map* l_Map = LaunchMap(l_MapTemplate.first);
        m_MapList[l_Map->GetID()][0] = l_Map;
    }
    return true;
}


void MapManager::AddMapTemplate(MapTemplate *p_MapTemplate)
{
    m_MapListTemplate[p_MapTemplate->GetID()] = p_MapTemplate;
}

Map* MapManager::GetMap(uint16 p_Id, uint16 p_InstanceID)
{
    if (m_MapList.find(p_Id) == m_MapList.end())
        return nullptr;

    if (m_MapList[p_Id].find(p_InstanceID) == m_MapList[p_Id].end())
        return nullptr;

    Map* l_Map = m_MapList[p_Id][p_InstanceID];
    return l_Map;
}

MapTemplate* MapManager::GetMapTemplate(uint16 p_ID)
{
    if (m_MapListTemplate.find(p_ID) == m_MapListTemplate.end())
        return nullptr;

    return m_MapListTemplate[p_ID];
}

bool MapManager::IsOnline(TypeUnit p_TypeID, uint16 p_UnitID)
{
    for (std::pair<uint16, std::map <uint16, Map*> > l_MapInstance : m_MapList)
    {
        for (std::pair<uint16, Map*> l_MapPair : l_MapInstance.second)
        {
            Map* l_Map = l_MapPair.second;

            if (l_Map == nullptr)
                continue;

            if (l_Map->GetUnit(p_TypeID, p_UnitID))
                return true;
        }
    }
    return false;
}

void MapManager::Update(sf::Time p_Diff)
{
    for (std::map<uint16, std::map<uint16, Map*>>::iterator l_It = m_MapList.begin(); l_It != m_MapList.end(); ++l_It)
    {
        for (std::map<uint16, Map*>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end(); ++l_Itr)
        {
            Map* l_Map = (*l_Itr).second;
            if (l_Map == nullptr)
                continue;

            l_Map->Update(p_Diff);
        }
    }

    for (std::map<uint16, std::map<uint16, Map*>>::iterator l_It = m_MapList.begin(); l_It != m_MapList.end(); ++l_It)
    {
        for (std::map<uint16, Map*>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end(); ++l_Itr)
        {
            Map* l_Map = (*l_Itr).second;
            if (l_Map == nullptr)
                continue;

            /// Switch Unit of map
            std::queue<Unit*>* l_UnitInterMapAction = l_Map->GetUnitInterMapAction(eInterMapAction::SwitchMap);
            while (l_UnitInterMapAction != nullptr && !l_UnitInterMapAction->empty())
            {
                Unit* l_Unit = l_UnitInterMapAction->front();
                Map* l_NewMap = GetMap(l_Unit->GetMapID(), l_Unit->GetInstanceID());

                if (l_NewMap == nullptr || l_NewMap->GetID() == l_Map->GetID()) /// If new map doesn't exist, we don't switch it
                {
                    l_Unit->SetMapID(l_Map->GetID());
                    l_UnitInterMapAction->pop();
                    continue;
                }
                l_Map->RemoveUnit(l_Unit);
                if (Player* l_Player = l_Unit->ToPlayer())
                    l_Player->GetSession()->SendSwitchMap(l_NewMap->GetID(), GetMapTemplate(l_NewMap->GetID())->GetFileName(), GetMapTemplate(l_NewMap->GetID())->GetFileChipset(), GetMapTemplate(l_NewMap->GetID())->GetName());
                l_NewMap->AddUnit(l_Unit);
                l_UnitInterMapAction->pop();
            }
        }
    }
}

Player* MapManager::GetPlayer(uint16 p_IdPlayer)
{
    for (std::pair<uint16, std::map <uint16, Map*> > l_MapInstance : m_MapList)
    {
        for (std::pair<uint16, Map*> l_MapPair : l_MapInstance.second)
        {
            Map* l_Map = l_MapPair.second;

            if (l_Map == nullptr)
                continue;

            Unit* l_Unit = l_Map->GetUnit(TypeUnit::PLAYER, p_IdPlayer);
            if (l_Unit == nullptr)
                continue;

            return l_Unit->ToPlayer();
        }
    }
    return nullptr;
}

std::vector<Player*> MapManager::GetAllPlayers()
{
    std::vector<Player*> l_ListPlayer;
    for (std::pair<uint16, std::map <uint16, Map*> > l_MapInstance : m_MapList)
    {
        for (std::pair<uint16, Map*> l_MapPair : l_MapInstance.second)
        {
            Map* l_Map = l_MapPair.second;

            if (l_Map == nullptr)
                continue;

            std::map<uint16, Unit*>* l_ListPlayerMap = l_Map->GetListUnitType(TypeUnit::PLAYER);
            for (auto l_Player : *l_ListPlayerMap)
                l_ListPlayer.push_back((l_Player.second)->ToPlayer());
        }
    }
    return l_ListPlayer;
}

int32 MapManager::StartInstance(uint16 p_MapID)
{
    uint16 i = 0;

    /// Search slot disponible
    while (m_MapList[p_MapID].find(i) != m_MapList[p_MapID].end())
        i++;

    //m_MapList[p_MapID][i] = new Map(p_MapID);

    return (uint16)i;
}