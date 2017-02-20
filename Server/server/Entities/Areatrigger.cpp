#include "Areatrigger.hpp"
#include "../Map/Map.hpp"

Areatrigger::Areatrigger(Map* p_Map, uint32 p_PosX, uint32 p_PosY, float p_Radius, eAreatriggerType p_Type) :
    DynamicObject(p_Map, p_PosX, p_PosY),
    m_Type(p_Type),
    m_Radius(p_Radius)
{
}

Areatrigger::~Areatrigger()
{
}

void Areatrigger::Update(sf::Time m_Diff)
{
    ;
}

void Areatrigger::UnitEnterInCase(Unit* p_Unit)
{
    p_Unit->TeleportTo(GetData(0), GetData(1), GetData(2), (Orientation)GetData(3));
}

void Areatrigger::SetData(uint8 p_ID, uint32 p_Data)
{
    m_Datas[p_ID] = p_Data;
}

uint32 Areatrigger::GetData(uint8 p_ID)
{
    if (m_Datas.find(p_ID) == m_Datas.end())
        return 0;
    return  m_Datas[p_ID];
}