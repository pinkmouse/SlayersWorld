#include "BGCapturePoint.hpp"
#include "../../../Global.hpp"
#include "../../../System/Spell/Aura.hpp"
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
    m_PointsRight = 0;
    m_PointsLeft = 0;
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
            (*l_It).second->TeleportTo(2, 0, 360, 380, Orientation::Up);
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

void BGCapturePoint::UnitUnaura(Unit* p_Unit, const uint16 & p_SpellID)
{
    if (m_Step != eBGState::STEP1)
        return;

    if (p_SpellID == AURA_FLAG_RIGHT && m_GroupManager->GetGroupForUnit(eGroupType::BG, p_Unit)->at(0) == TEAM_LEFT_NAME) ///< Right flag
    {
        PopFlagAtPoint((p_Unit->GetPosX() / TILE_SIZE) + (p_Unit->GetPosY() / TILE_SIZE) * p_Unit->GetMap()->GetSizeX(), m_FlagRight);
        //SendMsgToMap("L'équipe Left perd le tonneau !");
    }
    else if (p_SpellID == AURA_FLAG_LEFT && m_GroupManager->GetGroupForUnit(eGroupType::BG, p_Unit)->at(0) == TEAM_RIGHT_NAME) ///< Right flag
    {
        PopFlagAtPoint((p_Unit->GetPosX() / TILE_SIZE) + (p_Unit->GetPosY() / TILE_SIZE) * p_Unit->GetMap()->GetSizeX(), m_FlagLeft);
        //SendMsgToMap("L'équipe Right perd le tonneau !");
    }
}

void BGCapturePoint::UnitAddaura(Unit* p_Unit, const uint16 & p_SpellID, Aura* p_Aura)
{
    if (p_SpellID == AURA_FLAG_RIGHT || p_SpellID == AURA_FLAG_LEFT)
    {
        if (p_Unit->HasAuraType(eTypeAuraEffect::MOUNT))
            p_Unit->RemoveAuraType(eTypeAuraEffect::MOUNT);
    }
    if (p_Aura->GetSpellTemplate()->HasAuraEffect(eTypeAuraEffect::MOUNT) && (p_Unit->HasAura(AURA_FLAG_RIGHT) || p_Unit->HasAura(AURA_FLAG_LEFT)))
    {
        p_Unit->RemoveAura(AURA_FLAG_RIGHT);
        p_Unit->RemoveAura(AURA_FLAG_LEFT);
    }
}

void BGCapturePoint::TeamMakePoint(const uint8 & p_TeamID)
{
    std::string l_Msg = "L'équipe ";
    PacketExtraInterfaceData l_PacketData;


    switch (p_TeamID)
    {
    case TEAM_LEFTID:
        m_PointsLeft++;
        l_Msg += std::string(TEAM_LEFT_NAME) + " ";
        l_PacketData.BuildPacket(eExtraInterface::eBattelGroundUI, 1, 0, m_PointsLeft);
        break;
    case TEAM_RIGHTID:
        m_PointsRight++;
        l_Msg += std::string(TEAM_RIGHT_NAME) + " ";
        l_PacketData.BuildPacket(eExtraInterface::eBattelGroundUI, 2, 0, m_PointsRight);
        break;
    }
    if (m_PointsLeft >= MAX_FOINT || m_PointsRight >= MAX_FOINT)
        l_Msg += "gagne la partie !";
    else
        l_Msg += "pose le tonneau !";

    SendMsgToMap(l_Msg);
    SendToMap(l_PacketData.m_Packet);
    if (m_PointsLeft >= MAX_FOINT || m_PointsRight >= MAX_FOINT)
        EndBG();
}

bool BGCapturePoint::LauchTrapHandle(GameObject* p_Gob, Unit* p_Unit)
{
    if (p_Gob->GetGameObjectTemplate()->GetID() == GOB_FLAG_RIGHT && m_GroupManager->GetGroupForUnit(eGroupType::BG, p_Unit)->at(0) == TEAM_RIGHT_NAME)
    {
        if (p_Gob->GetPosX() / TILE_SIZE + ((p_Gob->GetPosY() / TILE_SIZE) * GetSizeX()) != BASE_FLAG_RIGHT)
        {
            p_Gob->Kill();
            PopFlagAtPoint(BASE_FLAG_RIGHT, m_FlagRight);
        }
        else if (p_Unit->HasAura(AURA_FLAG_LEFT))
        {
            p_Unit->RemoveAura(AURA_FLAG_LEFT);
            PopFlagAtPoint(BASE_FLAG_LEFT, m_FlagLeft);
            TeamMakePoint(TEAM_RIGHTID);
        }
        return false;
    }
    else if (p_Gob->GetGameObjectTemplate()->GetID() == GOB_FLAG_LEFT && m_GroupManager->GetGroupForUnit(eGroupType::BG, p_Unit)->at(0) == TEAM_LEFT_NAME)
    {
        if (p_Gob->GetPosX() / TILE_SIZE + ((p_Gob->GetPosY() / TILE_SIZE) * GetSizeX()) != BASE_FLAG_LEFT)
        {
            p_Gob->Kill();
            PopFlagAtPoint(BASE_FLAG_LEFT, m_FlagLeft);
        }
        else if (p_Unit->HasAura(AURA_FLAG_RIGHT))
        {
            p_Unit->RemoveAura(AURA_FLAG_RIGHT);
            PopFlagAtPoint(BASE_FLAG_RIGHT, m_FlagLeft);
            TeamMakePoint(TEAM_LEFTID);
        }
        return false;
    }
    if (p_Gob->GetGameObjectTemplate()->GetID() == GOB_FLAG_RIGHT)
        SendMsgToMap("L'équipe " + std::string(TEAM_LEFT_NAME) + " prend le tonneau !");
    if (p_Gob->GetGameObjectTemplate()->GetID() == GOB_FLAG_LEFT)
        SendMsgToMap("L'équipe " + std::string(TEAM_RIGHT_NAME) + " prend le tonneau !");

    return true;
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
    l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, 0, 1, (int16)(BG_MAX_TIME - (m_GlobalTimes / 1000 / 1000)));
    SendToMap(l_Packet.m_Packet);

    PacketExtraInterfaceData l_PackePointLeft;
    l_PackePointLeft.BuildPacket(eExtraInterface::eBattelGroundUI, 1, 0, m_PointsLeft);
    SendToMap(l_PackePointLeft.m_Packet);

    PacketExtraInterfaceData l_PackePointRight;
    l_PackePointRight.BuildPacket(eExtraInterface::eBattelGroundUI, 2, 0, m_PointsLeft);
    SendToMap(l_PackePointRight.m_Packet);

    m_Step = eBGState::STEP1;

    PopFlagAtPoint(BASE_FLAG_RIGHT, m_FlagRight);
    PopFlagAtPoint(BASE_FLAG_LEFT, m_FlagLeft);
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
        std::string l_Msg = "La partie commence dans : ";
        l_Msg += std::to_string(TIME_START_BG - (m_GlobalTimes / 1000 / 1000)) + "s";
        SendMsgToMap(l_Msg);
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

void BGCapturePoint::PopFlagAtPoint(const uint32 & p_CaseNb, GameObject* p_Gob)
{
    p_Gob->RespawnInCase(p_CaseNb);
}

void BGCapturePoint::AddUnit(Unit* p_Unit)
{
    Map::AddUnit(p_Unit);
    p_Unit->RemoveAllAura();

    if (p_Unit->GetType() == TypeUnit::GAMEOBJECT && p_Unit->ToGameObject()->GetGameObjectTemplate()->GetID() == GOB_FLAG_RIGHT && m_Step == eBGState::STEP0) ///< RIGHT FLAG
    {
        m_FlagRight = p_Unit->ToGameObject();
        m_FlagRight->Kill();
    }
    else if (p_Unit->GetType() == TypeUnit::GAMEOBJECT && p_Unit->ToGameObject()->GetGameObjectTemplate()->GetID() == GOB_FLAG_LEFT && m_Step == eBGState::STEP0) ///< LEFT FLAG
    {
        m_FlagLeft = p_Unit->ToGameObject();
        m_FlagLeft->Kill();
    }
    else if (p_Unit->IsPlayer())
    {
        m_SaveRespawnPoint[p_Unit->GetID()] = p_Unit->GetRespawnPoint();
        if (m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, TEAM_LEFT_NAME) < m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, TEAM_RIGHT_NAME))
        {
            p_Unit->TeleportTo(150, 380, Orientation::Right);
            p_Unit->SetRespawnPosition(WorldPosition(173, 570, GetID(), GetInstanceID(), Orientation::Left));
            m_GroupManager->AddUnitToGroup(eGroupType::BG, TEAM_LEFT_NAME, p_Unit);
        }
        else
        {
            p_Unit->TeleportTo(900, 380, Orientation::Left);
            p_Unit->SetRespawnPosition(WorldPosition(885, 168, GetID(), GetInstanceID(), Orientation::Left));
            m_GroupManager->AddUnitToGroup(eGroupType::BG, TEAM_RIGHT_NAME, p_Unit);
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
            l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, 0, 1, (int16)(BG_MAX_TIME - (m_GlobalTimes / 1000 / 1000)));
            l_Player->GetSession()->SendPacket(l_Packet.m_Packet);

            PacketExtraInterfaceData l_PackePointLeft;
            l_PackePointLeft.BuildPacket(eExtraInterface::eBattelGroundUI, 1, 0, m_PointsLeft);
            l_Player->GetSession()->SendPacket(l_PackePointLeft.m_Packet);

            PacketExtraInterfaceData l_PackePointRight;
            l_PackePointRight.BuildPacket(eExtraInterface::eBattelGroundUI, 2, 0, m_PointsLeft);
            l_Player->GetSession()->SendPacket(l_PackePointRight.m_Packet);
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

    Map::RemoveUnit(p_Unit);

    Player* l_Player = p_Unit->ToPlayer();
    if (l_Player == nullptr)
        return;

    WorldSocket* l_Session = l_Player->GetSession();
    if (l_Session == nullptr)
        return;

    PacketExtraInterface l_Packet;
    l_Packet.BuildPacket(eExtraInterface::eBattelGroundUI, false);
    l_Session->SendPacket(l_Packet.m_Packet);
    //if (p_Unit->IsPlayer())
      //  p_Unit->TeleportTo(2, 0, 360, 280, Orientation::Up);
}