#include "SpellManager.hpp"

SpellManager::SpellManager()
{
}

SpellManager::~SpellManager()
{
}

void SpellManager::AddSpell(SpellTemplate* p_Spell)
{
    m_Spells[p_Spell->GetID()] = p_Spell;
}

SpellTemplate* SpellManager::GetSpell(uint16 p_ID)
{
    if (m_Spells.find(p_ID) == m_Spells.end())
        return nullptr;
    return m_Spells[p_ID];
}

SpellEffect* SpellManager::GetSpellEffect(uint16 p_ID)
{
    if (m_SpellEffects.find(p_ID) == m_SpellEffects.end())
        return nullptr;
    return &m_SpellEffects[p_ID];
}

void SpellManager::AddSpellEffect(SpellEffect p_SpellEffect)
{
    m_SpellEffects[p_SpellEffect.m_ID] = p_SpellEffect;
}
