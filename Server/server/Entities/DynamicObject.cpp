#include "DynamicObject.hpp"
#include "../Map/Map.hpp"
#include "../World/PacketDefine.hpp"

DynamicObject::DynamicObject(uint16 p_ID, uint16 p_Entry, TypeUnit p_Type, Map* p_Map, uint32 p_PosX, uint32 p_PosY, int16 p_Skin) :
    Unit(p_ID, p_Type, eFactionType::Neutral)
{
    SetMap(p_Map);
    SetInWorld(true);
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    SetSkinID(p_Skin);
    SetMapID(p_Map->GetID());
    SetOrientation(Orientation::Up);
    m_SizeX = 16;
    m_SizeY = 16;
    m_Entry = p_Entry;
    m_RespawnPosition.SetMapID(p_Map->GetID());
    m_RespawnPosition.SetPosX(p_PosX);
    m_RespawnPosition.SetPosY(p_PosY);
    m_RespawnPosition.SetOrientation(Orientation::Up);
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::ActionFrom(Player* p_Player)
{
    ;
}

uint16 DynamicObject::GetEntry() const
{
    return m_Entry;
}

void DynamicObject::Respawn()
{
    Unit::Respawn();
    SetInWorld(true);

    SetResourceNb(eResourceType::Health, MAX_HEALTH);
    m_ResTimer = 0;

    /// Respawn dynamic object for players
    PacketUnitCreate l_Packet;
    l_Packet.BuildPacket(GetType(), GetID(), GetName(), GetSkinID(), GetSizeX(), GetSizeY(), GetSpeedUint8(), GetMapID(), GetPosition(), GetOrientation(), m_MovementHandler->IsInMovement(), IsBlocking());
    m_Map->SendToSet(l_Packet.m_Packet, this);

    std::vector<Unit*> l_ListUnit = m_Map->GetUnitsInCase(GetPosX(), GetPosY() - TILE_SIZE);
    if (!l_ListUnit.empty())
        UnitEnterInCase(l_ListUnit.front());
}

void DynamicObject::SetResourceNb(eResourceType p_Resource, uint8 p_Nb)
{
    Unit::SetResourceNb(p_Resource, p_Nb);
    if (IsDeath())
        Unspawn();
}

bool DynamicObject::IsBlocking() const
{
    return false;
}