#include "BGCapturePoint.hpp"
#include "../../../Global.hpp"

#define TEAM_LEFT 0
#define TEAM_RIGHT 1

BGCapturePoint::BGCapturePoint(uint16 p_InstanceID, uint16 p_MapID) :
    Map::Map(p_InstanceID, g_MapManager->GetMapTemplate(p_MapID))
{
    m_Step = eBGState::STEP0;
    m_GlobalTimes = 0;
    m_Finished = false;
    m_GroupManager = new GroupManager();
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

    for (std::map<uint16, Unit*>::iterator l_It = l_ListUnit->begin(); l_It != l_ListUnit->end(); l_It++)
    {
        (*l_It).second->TeleportTo(0, 0, 50, 50, Orientation::Down);
    }
    m_Step = eBGState::STEP2;
    m_Finished = true;
}

void BGCapturePoint::StartBG()
{
    printf("[BG] Starting BG\n");
    m_Step = eBGState::STEP1;
    if (m_Step == eBGState::STEP0 && m_GlobalTimes >= (5 * IN_MICROSECOND))
        StartBG();
}

void BGCapturePoint::Update(sf::Time p_Time)
{
    Map::Update(p_Time);
    m_GlobalTimes += p_Time.asMicroseconds();
    if (m_Step == eBGState::STEP0 && m_GlobalTimes >= (5 * IN_MICROSECOND))
        StartBG();
    if (m_Step == eBGState::STEP1 && m_GlobalTimes >= (20 * IN_MICROSECOND))
        EndBG();
}

void BGCapturePoint::AddUnit(Unit* p_Unit)
{
    Map::AddUnit(p_Unit);
    if (p_Unit->IsPlayer())
    {
        if (m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, "Left") < m_GroupManager->GetNBPlayerForGroup(eGroupType::BG, "Right"))
            m_GroupManager->AddUnitToGroup(eGroupType::BG, "Left", p_Unit);
        else
            m_GroupManager->AddUnitToGroup(eGroupType::BG, "Right", p_Unit);
    }
}

void BGCapturePoint::RemoveUnit(Unit* p_Unit)
{
    if (p_Unit->IsPlayer())
        p_Unit->TeleportTo(0, 0, 500, 500, Orientation::Down);
    Map::RemoveUnit(p_Unit);
}