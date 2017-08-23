#include <cstdio>
#include "MapManager.hpp"
#include "../Global.hpp"
#include "../System/WebHook.hpp"
#include "../World/WorldSocket.hpp"
#include "../World/PacketDefine.hpp"
#include "../System/Instance/BattleGround/BGCapturePoint.hpp"

#define NB_MAP 2

MapManager::MapManager()
{
    m_TimeStart = GetActualTime();
    m_Clock = 0;
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

    Map* l_Map = new Map(0, m_MapListTemplate[p_MapID]);
    l_Map->Initialize();
    return l_Map;
}

bool MapManager::LaunchWorldsMap()
{
    for (auto l_MapTemplate : m_MapListTemplate)
    {
        if (l_MapTemplate.second->IsInstance())
            continue;
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

void MapManager::UpdateQueues(sf::Time p_Diff)
{
    for (std::map<uint16, std::pair<BGTemplate*, std::vector<Player*>>>::iterator l_Itr = m_BGListTemplate.begin() ; l_Itr != m_BGListTemplate.end(); l_Itr++)
    {
        /// Remove null players
        for (std::vector<Player*>::iterator l_It = (*l_Itr).second.second.begin(); l_It != (*l_Itr).second.second.end();)
        {
            if ((*l_It) == nullptr)
                l_It = (*l_Itr).second.second.erase(l_It);
            else
                l_It++;
        }

        /// Check actual bg
        for (auto l_Map : m_MapList[(*l_Itr).second.first->m_MapID])
        {
            for (std::vector<Player*>::iterator l_It = (*l_Itr).second.second.begin(); l_Map.second->GetNbUnitType(TypeUnit::PLAYER) < (*l_Itr).second.first->m_MaxPlayer &&  l_It != (*l_Itr).second.second.end();)
            {
                (*l_It)->TeleportTo(l_Map.second->GetID(), l_Map.second->GetInstanceID(), 50, 50, Orientation::Down);
                l_It = (*l_Itr).second.second.erase(l_It);
            }
        }

        /// Check Queues
        if ((*l_Itr).second.second.size() >= (*l_Itr).second.first->m_MinPlayer)
        {
            Map* l_NewBGInstance = nullptr;
            uint16 l_InstanceID = GetValidInstanceIDForMap((*l_Itr).second.first->m_MapID);
            switch ((*l_Itr).first)
            {
                case 0 :
                    l_NewBGInstance = new BGCapturePoint(l_InstanceID, (*l_Itr).second.first->m_MapID);
                    l_NewBGInstance->Initialize();
                default :
                break;
            }
            if (l_NewBGInstance != nullptr)
            {
                uint16 l_NbPlayers = 0;

                m_MapList[l_NewBGInstance->GetID()][l_InstanceID] = l_NewBGInstance;
                for (std::vector<Player*>::iterator l_It = (*l_Itr).second.second.begin() ; l_It != (*l_Itr).second.second.end();)
                {
                    if (l_NbPlayers > (*l_Itr).second.first->m_MaxPlayer)
                        break;
                    (*l_It)->TeleportTo(l_NewBGInstance->GetID(), l_InstanceID, 50, 50, Orientation::Down);
                    l_It = (*l_Itr).second.second.erase(l_It);
                    l_NbPlayers++;
                }
            }
        }
    }
}

void MapManager::Update(sf::Time p_Diff)
{
    UpdateQueues(p_Diff);
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
        for (std::map<uint16, Map*>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end();)
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

                if (l_NewMap == nullptr || (l_NewMap->GetID() == l_Map->GetID() && l_NewMap->GetInstanceID() == l_Map->GetInstanceID())) /// If new map doesn't exist, we don't switch it
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
            if (l_Map->IsFinish()) ///< For Instances
            {
                delete l_Map;
                l_Itr = (*l_It).second.erase(l_Itr);
            }
            else
                l_Itr++;
        }
    }

    m_Clock += p_Diff.asMicroseconds();
    if (m_Clock >= (SEND_TIME_CLOCK_WEBHOOK * 60) * IN_MICROSECOND)
    {
        SWTime l_Time = ConvertTimeToSWTime(difftime(GetActualTime(), g_MapManager->GetTimeStart()));
        WebHook::sendMsg(g_Config->GetValue("WebhookUrl"), "Serveur " + g_Config->GetValue("ServerName") + " allume depuis " + std::to_string(l_Time.m_Days) + "j " + std::to_string(l_Time.m_Hours) + "h " + std::to_string(l_Time.m_Minutes) + "m " + std::to_string(l_Time.m_Seconds) + "s  Clock " + std::to_string(p_Diff.asMicroseconds()) + "  Total Seconds " + std::to_string(l_Time.m_TotalSeconds));
        m_Clock = 0;
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

uint16 MapManager::GetTotalPlayers()
{
    uint16 l_TotalNb = 0;
    for (std::pair<uint16, std::map <uint16, Map*> > l_MapInstance : m_MapList)
    {
        for (std::pair<uint16, Map*> l_MapPair : l_MapInstance.second)
        {
            Map* l_Map = l_MapPair.second;

            if (l_Map == nullptr)
                continue;

            std::map<uint16, Unit*>* l_ListPlayerMap = l_Map->GetListUnitType(TypeUnit::PLAYER);
            l_TotalNb += l_ListPlayerMap->size();
        }
    }
    return l_TotalNb;
}

void MapManager::SaveAllPlayers()
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
                (l_Player.second)->ToPlayer()->Save();
        }
    }
}


void MapManager::AddBGTemplate(BGTemplate* p_BGTemplate)
{
    m_BGListTemplate[p_BGTemplate->m_ID].first = p_BGTemplate;
}

void MapManager::AddPlayerToQueue(uint16 p_BGID, Player* p_Player)
{
    if (m_BGListTemplate.find(p_BGID) == m_BGListTemplate.end())
        return;

    uint8 l_MinPlayer = m_BGListTemplate[p_BGID].first->m_MinPlayer;

    if (std::find(m_BGListTemplate[p_BGID].second.begin(), m_BGListTemplate[p_BGID].second.end(), p_Player) != m_BGListTemplate[p_BGID].second.end())
    {
        PacketSrvPlayerMsg l_PacketNewPLayer;
        std::string l_MsgError = "Vous êtes déjà inscrit pour ce champs de bataille : " + std::to_string(m_BGListTemplate[p_BGID].second.size()) + "/" + std::to_string(l_MinPlayer);
        l_PacketNewPLayer.BuildPacket(l_MsgError);
        if (p_Player->GetSession())
            p_Player->GetSession()->send(l_PacketNewPLayer.m_Packet);
        return;
    }

    PacketSrvPlayerMsg l_Packet;
    std::string l_Msg = "Le joueur " + p_Player->GetName() + " à rejoin la file d'attente : " + std::to_string(m_BGListTemplate[p_BGID].second.size() + 1) + "/" + std::to_string(l_MinPlayer);
    l_Packet.BuildPacket(l_Msg);
    for (uint8 i = 0; i < m_BGListTemplate[p_BGID].second.size(); i++)
    {
        Player* l_Player = m_BGListTemplate[p_BGID].second[i];
        if (l_Player == nullptr)
            continue;
        if (p_Player->GetSession())
            l_Player->GetSession()->send(l_Packet.m_Packet);
    }

    PacketSrvPlayerMsg l_PacketNewPLayer;
    l_Msg = "Vous êtes inscrit : " + std::to_string(m_BGListTemplate[p_BGID].second.size() + 1) + "/" + std::to_string(l_MinPlayer);
    l_PacketNewPLayer.BuildPacket(l_Msg);
    if (p_Player->GetSession())
        p_Player->GetSession()->send(l_PacketNewPLayer.m_Packet);

    m_BGListTemplate[p_BGID].second.push_back(p_Player);
}

uint16 MapManager::GetValidInstanceIDForMap(uint16 p_MapID)
{
    uint16 l_InstanceID = 0;
    std::map<uint16, Map*>::iterator l_It;
    while (1)
    {
        l_It = m_MapList[p_MapID].find(l_InstanceID);
        if (l_It == m_MapList[p_MapID].end() || (*l_It).second == nullptr)
            break;
        l_InstanceID++;
    }
    return l_InstanceID;
}

time_t MapManager::GetTimeStart() const
{
    return m_TimeStart;
}