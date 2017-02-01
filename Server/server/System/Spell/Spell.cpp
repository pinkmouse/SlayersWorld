#include "Spell.hpp"

Spell::Spell(uint16 p_ID) :
    m_ID(p_ID)
{
    m_Level = 0;
    m_VisualID = 0;
}

Spell::~Spell()
{
}

uint16 Spell::GetID()
{
    return m_ID;
}

uint8 Spell::GetLevel()
{
    return m_Level;
}

uint16 Spell::GetVisualID()
{
    return m_VisualID;
}

std::vector<SpellEffect*>* Spell::GetListEffect()
{
    return &m_ListEffect;
}

void Spell::SetLevel(uint8 p_Level)
{
    m_Level = p_Level;
}
void Spell::SetVisualID(uint16 p_VisualID)
{
    m_VisualID = p_VisualID;
}

void Spell::AddSpellEffect(SpellEffect* p_SpellEffect)
{
    m_ListEffect.push_back(p_SpellEffect);
}