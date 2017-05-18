#include "Case.hpp"
#include "../Entities/Areatrigger.hpp"

Case::Case(uint32 p_ID, uint16 p_X, uint16 p_Y) :
	m_ID(p_ID),
	m_Block(false)
{
    SetPosX(p_X);
    SetPosY(p_Y);
}

Case::~Case()
{
}

uint32 Case::GetID() const
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

void Case::AddZone(Zone* p_Zone)
{
    m_ZoneList.push_back(p_Zone);
}

bool Case::IsInZoneType(eTypeZone p_Type)
{
    for (uint8 i = 0; i < m_ZoneList.size(); ++i)
    {
        if (m_ZoneList[i]->m_TypeID == p_Type && m_ZoneList[i]->m_enabled)
            return true;
    }
    return false;
}

bool Case::HasZone(uint16 p_ZoneId)
{
    for (uint8 i = 0; i < m_ZoneList.size(); ++i)
    {
        if (m_ZoneList[i]->m_ID == p_ZoneId)
            return true;
    }
    return false;
}

std::vector<Zone*> Case::CompareZones(Case* p_Case)
{
    std::vector<Zone*> m_ZoneList;
    for (uint8 i = 0; i < m_ZoneList.size(); ++i)
    {
        if (!p_Case->HasZone(m_ZoneList[i]->m_ID))
            m_ZoneList.push_back(m_ZoneList[i]);
    }
    return m_ZoneList;
}

void Case::UnitEnterInCase(Unit* p_Unit, Case* p_OlderCase)
{
    std::vector<Zone*> m_ZoneList;
    m_ZoneList = CompareZones(p_OlderCase);

    if (!m_ZoneList.empty())
        printf("Enter in zone");
    for (uint8 i = 0; i < m_DynamicObjectList.size(); ++i)
        m_DynamicObjectList[i]->UnitEnterInCase(p_Unit);
}