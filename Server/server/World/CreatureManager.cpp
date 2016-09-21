#include "CreatureManager.hpp"


CreatureManager::CreatureManager()
{
}


CreatureManager::~CreatureManager()
{
}

void CreatureManager::AddCreatureTemplate(const CreatureTemplate & p_CreatureTemplate)
{
    m_CreatureTemplateMap[p_CreatureTemplate.m_Entry] = p_CreatureTemplate;
}

CreatureTemplate CreatureManager::GetCreatureTemplate(uint32 p_Entry)
{
    return m_CreatureTemplateMap[p_Entry];
}