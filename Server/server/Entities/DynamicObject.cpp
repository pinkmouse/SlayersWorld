#include "DynamicObject.hpp"


DynamicObject::DynamicObject(Case* p_Case) :
    m_Case(p_Case),
    m_InWorld(true)
{
}

DynamicObject::~DynamicObject()
{
}

void DynamicObject::SetInWorld(bool p_InWorld)
{
    m_InWorld = p_InWorld;
}

bool DynamicObject::IsInWorld() const
{
    return m_InWorld;
}