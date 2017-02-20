#include "DynamicObject.hpp"


DynamicObject::DynamicObject(Map* p_Map, uint32 p_PosX, uint32 p_PosY) :
    m_Map(p_Map),
    m_InWorld(true)
{
    SetPosX(p_PosX);
    SetPosY(p_PosY);
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