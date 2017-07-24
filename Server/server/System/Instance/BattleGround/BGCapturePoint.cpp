#include "BGCapturePoint.hpp"
#include "../../../Global.hpp"
#include "../../../World/PacketDefine.hpp"
#include "../../../World/WorldSocket.hpp"

#define TEAM_LEFT 0
#define TEAM_RIGHT 1

BGCapturePoint::BGCapturePoint(uint16 p_InstanceID, uint16 p_MapID) :
    Map::Map(p_InstanceID, g_MapManager->GetMapTemplate(p_MapID))
{
    m_Step = eBGState::STEP0;
    m_GlobalTimes = 0;
    m_Finished = false;
    m_GroupManager = new GroupManager();
    m_Chrono = 0;
}

BGCapturePoint::~BGCapturePoint()
{
}

bool BGCapturePoint::IsFinish()
{
    if (GetNbUnitType(TypeUnit::PLAYER) > 0)
        return false;
    if (GetNbUnitType(TypeUnit::PLAYER) > 0 && m_Finished)
        EndBG();
    return m_Finished;
}


void BGCapturePoint::EndBG()
{
    std::map<uint16, Unit*>* l_ListUnit = GetListUnitType(TypeUnit::PLAYER);
    printf("[BG] Ending BG\n");

    if (l_ListUnit != nullptr)
    {
        for (std::map<uint16, Unit*>::iterator l_It = l_ListUnit->begin(); l_It != l_ListUnit->end(); l_It++)
        {
            Player* l_Player = (*l_It).second->ToPlayer();
            if (l_Player == nullptr)
                continue;

            l_Player->TeleportTo(2, 0, 360, 380, Orientation::Up);
            WorldSocket* l_Session = l_Player->GetSession();
            if (l_Session == nullptr)
                continue;

            PacketExtraInterface l_Packet;
            l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, false);
            l_Session->SendPacket(l_Packet.m_Packet);
        }
    }
    m_Step = eBGState::STEP2;
    m_Finished = true;
}

void BGCapturePoint::OpenGraveyard()
{
    std::vector<Unit*> l_UnitList = GetAllUnitWithEntry(TypeUnit::GAMEOBJECT, 14);
    for (uint16 i = 0; i < l_UnitList.size(); i++)
    {
        if (l_UnitList[i] != nullptr)
            l_UnitList[i]->Kill();
    }
}

void BGCapturePoint::StartBG()
{
    printf("[BG] Starting BG\n");
    std::vector<Unit*> l_UnitList = GetAllUnitWithEntry(TypeUnit::GAMEOBJECT, 8);
    for (uint16 i = 0; i < l_UnitList.size(); i++)
    {
        if (l_UnitList[i] != nullptr)
            l_UnitList[i]->Kill();
    }

    PacketExtraInterfaceData l_Packet;
    std::string l_Time;
    l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, 0, 1, (uint8)(BG_MAX_TIME - (m_GlobalTimes / 1000 / 1000)));
    SendToMap(l_Packet.m_Packet);

    m_Step = eBGState::STEP1;
}

void BGCapturePoint::Chrono()
{
    if (m_GlobalTimes >= (((TIME_START_BG - 10) + m_Chrono) * IN_MICROSECOND))
    {
        PacketWarningMsg l_Packet;
        std::string l_Time;
        l_Time = std::to_string(10 - m_Chrono);
        l_Packet.BuildPacket(eTypeWarningMsg::BigMsg, l_Time);
        SendToMap(l_Packet.m_Packet);
        m_Chrono++;
    }
}

void BGCapturePoint::ChronoPeriodic()
{
    if ((m_GlobalTimes / 1000 / 1000) + 15 > TIME_START_BG)
        m_Chrono = 0;
    else if ((m_GlobalTimes / 1000 / 1000) / 15 >= m_Chrono)
    {
        PacketSrvPlayerMsg l_Packet;
        std::string l_Msg = "La partie commence dans : ";
        l_Msg += std::to_string(TIME_START_BG - (m_GlobalTimes / 1000 / 1000)) + "s";
        l_Packet.BuildPacket(l_Msg);
        SendToMap(l_Packet.m_Packet);
        m_Chrono++;
    }
}

void BGCapturePoint::UpdateBGStep1(sf::Time p_Time)
{
    if ((m_GlobalTimes / 1000 / 1000 / 60) != (m_GlobalTimes - p_Time.asMicroseconds()) / 1000 / 1000 / 60)
        OpenGraveyard();
}

void BGCapturePoint::Update(sf::Time p_Time)
{
    Map::Update(p_Time);
    m_GlobalTimes += p_Time.asMicroseconds();

    /// STEP0
    if (m_Step == eBGState::STEP0 && m_GlobalTimes < ((TIME_START_BG - 10) * IN_MICROSECOND))
        ChronoPeriodic();
    else if (m_Step == eBGState::STEP0 && m_GlobalTimes >= ((TIME_START_BG - 10) * IN_MICROSECOND) && m_GlobalTimes < (TIME_START_BG * IN_MICROSECOND))
        Chrono();
    else if (m_Step == eBGState::STEP0 && m_GlobalTimes >= (TIME_START_BG * IN_MICROSECOND))
        StartBG();
    else if (m_Step == eBGState::STEP1 && m_GlobalTimes < (BG_MAX_TIME * IN_MICROSECOND))
        UpdateBGStep1(p_Time);
    /// STEP1
    else if (m_Step == eBGState::STEP1 && m_GlobalTimes >= ((BG_MAX_TIME * IN_MICROSECOND)))
        EndBG();
}

void BGCapturePoint::AddUnit(Unit* p_Unit)
{
    Map::AddUnit(p_Unit);
    p_Unit->RemoveAllAura();
    if (p_Unit->IsPlayer())
    {
        m_SaveRespawnPoint[p_Unit->GetID()] = p_Unit->GetRespawnPoint();
        if (m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, "Left") < m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, "Right"))
        {
            p_Unit->TeleportTo(150, 380, Orientation::Right);
            p_Unit->SetRespawnPosition(WorldPosition(885, 168, GetID(), GetInstanceID(), Orientation::Left));
            m_GroupManager->AddUnitToGroup(eGroupType::BG, "Left", p_Unit);
        }
        else
        {
            p_Unit->TeleportTo(900, 380, Orientation::Left);
            p_Unit->SetRespawnPosition(WorldPosition(173, 570, GetID(), GetInstanceID(), Orientation::Left));
            m_GroupManager->AddUnitToGroup(eGroupType::BG, "Right", p_Unit);
        }

        Player* l_Player = p_Unit->ToPlayer();
        if (l_Player == nullptr)
            return;

        WorldSocket* l_Session = l_Player->GetSession();
        if (l_Session == nullptr)
            return;

        PacketExtraInterface l_Packet;
        l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, true);
        l_Session->SendPacket(l_Packet.m_Packet);
        if (m_Step == eBGState::STEP0)
            l_Player->SendMsg("La partie commence dans : " + std::to_string(TIME_START_BG - (m_GlobalTimes / 1000 / 1000)) + "s");
        else
        {
            PacketExtraInterfaceData l_Packet;
            std::string l_Time;
            l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, 0, 1, (uint8)((m_GlobalTimes / 1000 / 1000) - TIME_START_BG));
            l_Player->GetSession()->SendPacket(l_Packet.m_Packet);
        }
    }
}

void BGCapturePoint::RemoveUnit(Unit* p_Unit)
{
    p_Unit->RemoveAllAura();

    std::map<uint16, WorldPosition>::iterator l_It = m_SaveRespawnPoint.find(p_Unit->GetID());
    if (l_It != m_SaveRespawnPoint.end())
    {
        p_Unit->SetRespawnPosition(m_SaveRespawnPoint[p_Unit->GetID()]);
        m_SaveRespawnPoint.erase(l_It);
    }
    //if (p_Unit->IsPlayer())
      //  p_Unit->TeleportTo(2, 0, 360, 280, Orientation::Up);
    Map::RemoveUnit(p_Unit);
}