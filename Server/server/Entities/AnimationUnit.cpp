#include "AnimationUnit.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"

AnimationUnit::AnimationUnit(uint16 p_ID, AnimationUnitTemplate* p_AnimationUnitTemplate, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY) :
    Unit(p_ID, TypeUnit::ANIMATIONUNIT, eFactionType::Neutral)
{
    m_Entry = p_AnimationUnitTemplate->m_Entry;
    SetMapID(p_MapID);
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_SkinID = p_AnimationUnitTemplate->m_SkinID;
    m_Name = p_AnimationUnitTemplate->m_Name;
    m_AnimationUnitTemplate = p_AnimationUnitTemplate;

    m_RespawnPosition.SetMapID(p_MapID);
    m_RespawnPosition.SetPosX(p_PosX);
    m_RespawnPosition.SetPosY(p_PosY);

    m_DiffMovementTime = 0;
    m_DiffPlayAnim = (p_AnimationUnitTemplate->m_StopTimeMin + (rand() % (uint16)((float)(p_AnimationUnitTemplate->m_StopTimeMax - p_AnimationUnitTemplate->m_StopTimeMin)))) * 1000;
    m_Diff = 0;
}

void AnimationUnit::UpdateOrientation(Orientation p_Orientation)
{
    if (p_Orientation == m_MovementHandler->GetOrientation())
        return;

    m_MovementHandler->SetOrientation(p_Orientation);
    PacketUpdateOrientation l_Packet;
    l_Packet.BuildPacket((uint8)GetType(), GetID(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
}

AnimationUnit::~AnimationUnit()
{
}

void AnimationUnit::LaunchAnim()
{
    PacketStartAttack l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(),GetPosition(), GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, this);
    m_MovementHandler->SetOrientation(Orientation::Down);
}

void AnimationUnit::Update(sf::Time p_Time)
{
    Unit::Update(p_Time);
    m_Diff += p_Time.asMicroseconds();
    while (m_Diff >= m_DiffPlayAnim)
    {
        LaunchAnim();
        m_DiffPlayAnim = (m_AnimationUnitTemplate->m_StopTimeMin + (rand() % (uint16)((float)(m_AnimationUnitTemplate->m_StopTimeMax - m_AnimationUnitTemplate->m_StopTimeMin)))) * 1000;
        m_Diff = 0;
    }
}