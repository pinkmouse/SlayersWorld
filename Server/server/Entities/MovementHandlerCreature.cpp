#include "MovementHandlerCreature.hpp"
#include "../World/PacketDefine.hpp"
#include "../Map/Map.hpp"

MovementHandlerCreature::MovementHandlerCreature(Unit* p_Creature, uint8 p_SizeX, uint8 p_SizeY) :
    m_Owner(p_Creature),
    MovementHandler::MovementHandler(p_SizeX, p_SizeY, TypeUnit::CREATURE)
{
}

MovementHandlerCreature::~MovementHandlerCreature()
{
}

void MovementHandlerCreature::StartMovement(Orientation p_Orientation)
{
    MovementHandler::StartMovement(p_Orientation);
    PacketGoDirection l_Packet;
    l_Packet.BuildPacket(m_Owner->GetType(), m_Owner->GetID(), m_Owner->GetPosition(), m_Owner->GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, m_Owner);
}

void MovementHandlerCreature::StopMovement()
{
    MovementHandler::StopMovement();
    PacketStopMovement l_Packet;
    l_Packet.BuildPacket(m_Owner->GetType(), m_Owner->GetID(), m_Owner->GetPosition(), m_Owner->GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, m_Owner);
}

void MovementHandlerCreature::StartAttack()
{
    MovementHandler::StartAttack();
    PacketStartAttack l_Packet;
    l_Packet.BuildPacket(m_Owner->GetType(), m_Owner->GetID(), m_Owner->GetPosition(), m_Owner->GetOrientation());
    m_Map->SendToSet(l_Packet.m_Packet, m_Owner);
}

void MovementHandlerCreature::StopAttack()
{
    MovementHandler::StopAttack();
    PacketStopAttack l_Packet;
    l_Packet.BuildPacket(m_Owner->GetType(), m_Owner->GetID());
    m_Map->SendToSet(l_Packet.m_Packet, m_Owner);
}