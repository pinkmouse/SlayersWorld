#include "Areatrigger.hpp"
#include "../Map/Map.hpp"

Areatrigger::Areatrigger(Map* p_Map, uint32 p_PosX, uint32 p_PosY, float p_Radius, eAreatriggerType p_Type) :
    DynamicObject(p_Map, p_PosX, p_PosY),
    m_Type(p_Type),
    m_Radius(p_Radius)
{
    m_Timer = 0;
}

Areatrigger::~Areatrigger()
{
}

void Areatrigger::Update(sf::Time m_Diff)
{
    if (m_Type == eAreatriggerType::AreatriggerPeriodicDamage)
    {
        m_Timer += m_Diff.asMicroseconds();
        while (m_Timer >= GetData(0) * IN_MILLISECOND)
        {
            std::vector<Unit*> l_ListUnit = GetMap()->GetUnitsInRadius(this, 0.0f, 1.0f, true);
            for (uint8 i = 0; i < l_ListUnit.size(); i++)
            {
                DamageInfo l_DamageInfo;
                l_DamageInfo.m_Damage = GetData(1);
                l_ListUnit[i]->DealDamage(l_ListUnit[i], l_DamageInfo);
            }
            m_Timer -= GetData(0) * IN_MILLISECOND;
        }
    }
}

bool Areatrigger::CanBeWalk()
{
    if (m_Type == eAreatriggerType::AreatriggerTeleport)
        return false;
    return true;
}

void Areatrigger::UnitEnterInCase(Unit* p_Unit)
{
    if (m_Type == eAreatriggerType::AreatriggerTeleport)
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