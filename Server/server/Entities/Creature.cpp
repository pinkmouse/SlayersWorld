#include "Creature.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"

Creature::Creature(uint16 p_ID, uint16 p_Entry, CreatureTemplate p_CreatureTemplate, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY) :
    Unit(p_ID, TypeUnit::CREATURE)
{
    m_Entry = p_Entry;
    m_MapID = p_MapID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_SkinID = p_CreatureTemplate.m_SkinID;
    m_Level = p_CreatureTemplate.m_Level;
    m_Name = p_CreatureTemplate.m_Name;
    m_CreatureTemplate = p_CreatureTemplate;
    m_DiffMovementTime = 0;
}


Creature::~Creature()
{
}

void Creature::Update(sf::Time p_Diff)
{
    Unit::Update(p_Diff);

    m_DiffMovementTime += p_Diff.asMicroseconds();

    while (m_DiffMovementTime > (10 * IN_MICROSECOND)) ///< 1000 because microsecond
    {
       /* if (m_MovementHandler->IsInMovement())
            m_MovementHandler->StopMovement();
        else
        {
            if (m_MovementHandler->GetOrientation() != (Orientation)Left)
            {
                m_MovementHandler->StartMovement((Orientation)Left);
                WorldPacket l_Packet;
                uint8 l_ID = SMSG::S_UnitGoDirection;

                l_Packet << l_ID << TypeUnit::CREATURE << GetID() << GetPosX() << GetPosY() << GetOrientation();
                m_Map->SendToSet(l_Packet, this);
            }
            else
            {
                m_MovementHandler->StartMovement((Orientation)Right);
                WorldPacket l_Packet;
                uint8 l_ID = SMSG::S_UnitGoDirection;

                l_Packet << l_ID << TypeUnit::CREATURE << GetID() << GetPosX() << GetPosY() << GetOrientation();
                m_Map->SendToSet(l_Packet, this);
            }
            m_DiffMovementTime = 0;
        }*/
    }
}