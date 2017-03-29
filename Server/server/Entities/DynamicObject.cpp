#include "DynamicObject.hpp"
#include "../Map/Map.hpp"

DynamicObject::DynamicObject(uint16 p_ID, uint16 p_Entry, TypeUnit p_Type, Map* p_Map, uint32 p_PosX, uint32 p_PosY) :
    Unit(p_ID, p_Type, eFactionType::Neutral),
    m_Map(p_Map),
    m_InWorld(true)
{
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    SetMapID(p_Map->GetID());
    SetOrientation(Orientation::Up);
    m_SizeX = 16;
    m_SizeY = 16;
}

DynamicObject::~DynamicObject()
{
}

Map* DynamicObject::GetMap() const
{
    return m_Map;
}

void DynamicObject::SetInWorld(bool p_InWorld)
{
    m_InWorld = p_InWorld;
}

bool DynamicObject::IsInWorld() const
{
    return m_InWorld;
}