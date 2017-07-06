#include "UnitManager.hpp"


UnitManager::UnitManager()
{
}


UnitManager::~UnitManager()
{
}

void UnitManager::AddCreatureTemplate(const CreatureTemplate & p_CreatureTemplate)
{
    m_CreatureTemplateMap[p_CreatureTemplate.m_Entry] = p_CreatureTemplate;
}

CreatureTemplate* UnitManager::GetCreatureTemplate(uint32 p_Entry)
{
    return &m_CreatureTemplateMap[p_Entry];
}

void UnitManager::AddGossip(const Gossip & p_Gossip)
{
    m_MapGossip[p_Gossip.m_TypeUnit][p_Gossip.m_UnitEntry].push_back(p_Gossip);
}

std::vector<Gossip>* UnitManager::GetGossipListFor(TypeUnit p_TypeUnit, uint16 p_UnitEntry)
{
    return &m_MapGossip[p_TypeUnit][p_UnitEntry];
}
