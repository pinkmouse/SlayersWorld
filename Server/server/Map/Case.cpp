#include "Case.hpp"
#include "../Entities/Areatrigger.hpp"

Case::Case(uint16 p_ID, uint16 p_X, uint16 p_Y) :
	m_ID(p_ID),
	m_Block(false)
{
    SetPosX(p_X);
    SetPosY(p_Y);
}

Case::~Case()
{
}

uint16 Case::GetID() const
{
    return m_ID;
}

bool Case::IsBlocking() const
{
    if (m_Block)
        return true;

    for (uint8 i = 0; i < m_DynamicObjectList.size(); ++i)
    {
        if (m_DynamicObjectList[i]->IsInWorld() && m_DynamicObjectList[i]->IsBlocking())
            return true;
    }
    return false;
}

bool Case::CanBeWalk()
{
    for (uint8 i = 0; i < m_DynamicObjectList.size(); ++i)
    {
        if (!m_DynamicObjectList[i]->CanBeWalk())
            return false;
    }
    return true;
}

void Case::SetBlock(bool p_Block)
{
	m_Block = p_Block;
}

void Case::AddDynamicOject(DynamicObject* p_DynamicObject)
{
    m_DynamicObjectList.push_back(p_DynamicObject);
}

void Case::UnitEnterInCase(Unit* p_Unit)
{
    for (uint8 i = 0; i < m_DynamicObjectList.size(); ++i)
        m_DynamicObjectList[i]->UnitEnterInCase(p_Unit);
}