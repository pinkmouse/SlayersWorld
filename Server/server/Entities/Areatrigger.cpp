#include "Areatrigger.hpp"
#include "../Map/Map.hpp"

AreatriggerTemplate::AreatriggerTemplate() :
    m_Id(0),
    m_Type(eAreatriggerType::AreatriggerPeriodicDamage),
    m_Radius(0.0f)
{
}

AreatriggerTemplate::AreatriggerTemplate(uint16 p_Id, float p_Radius, eAreatriggerType p_Type) :
    m_Id(p_Id),
    m_Type(p_Type),
    m_Radius(p_Radius)
{
}

AreatriggerTemplate::~AreatriggerTemplate()
{
}

uint16 AreatriggerTemplate::GetID() const
{
    return m_Id;
}

void AreatriggerTemplate::SetData(uint8 p_ID, uint32 p_Data)
{
    m_Datas[p_ID] = p_Data;
}

uint32 AreatriggerTemplate::GetData(uint8 p_ID)
{
    if (m_Datas.find(p_ID) == m_Datas.end())
        return 0;
    return  m_Datas[p_ID];
}

float AreatriggerTemplate::GetRadius() const
{
    return m_Radius;
}

eAreatriggerType AreatriggerTemplate::GetType() const
{
    return m_Type;
}

Areatrigger::Areatrigger(uint16 p_Id, Map* p_Map, uint32 p_PosX, uint32 p_PosY, AreatriggerTemplate* p_AreatriggerTemplate) :
    DynamicObject(p_Id, p_AreatriggerTemplate->GetID(), TypeUnit::AREATRIGGER ,p_Map, p_PosX, p_PosY),
    m_AreatriggerTemplate(p_AreatriggerTemplate)
{
    m_Timer = 0;
    m_SkinID = 1;
}

Areatrigger::~Areatrigger()
{
}

void Areatrigger::Update(sf::Time m_Diff)
{
    if (m_AreatriggerTemplate->GetType() == eAreatriggerType::AreatriggerPeriodicDamage)
    {
        m_Timer += m_Diff.asMicroseconds();
        while (m_Timer >= m_AreatriggerTemplate->GetData(0) * IN_MILLISECOND)
        {
            std::vector<Unit*> l_ListUnit = GetMap()->GetUnitsInRadius(this, 0.0f, 1.0f, true);
            for (uint8 i = 0; i < l_ListUnit.size(); i++)
            {
                DamageInfo l_DamageInfo;
                l_DamageInfo.m_Damage = m_AreatriggerTemplate->GetData(1);
                l_ListUnit[i]->DealDamage(l_ListUnit[i], l_DamageInfo);
            }
            m_Timer -= m_AreatriggerTemplate->GetData(0) * IN_MILLISECOND;
        }
    }
}

bool Areatrigger::CanBeWalk()
{
    if (m_AreatriggerTemplate->GetType() == eAreatriggerType::AreatriggerTeleport)
        return false;
    return true;
}

void Areatrigger::UnitEnterInCase(Unit* p_Unit)
{
    if (m_AreatriggerTemplate->GetType() == eAreatriggerType::AreatriggerTeleport)
        p_Unit->TeleportTo(m_AreatriggerTemplate->GetData(0), m_AreatriggerTemplate->GetData(1), m_AreatriggerTemplate->GetData(2), (Orientation)m_AreatriggerTemplate->GetData(3));
}

bool Areatrigger::IsBlocking()
{
    return false;
}