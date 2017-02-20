#include "Areatrigger.hpp"

Areatrigger::Areatrigger(Case* p_Case, eAreatriggerType p_Type) :
    DynamicObject(p_Case),
    m_Type(p_Type)
{
}

Areatrigger::~Areatrigger()
{
}

void Areatrigger::Update(sf::Time m_Diff)
{

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