#include "SpellTemplate.hpp"

SpellTemplate::SpellTemplate(uint16 p_ID) :
    m_ID(p_ID)
{
    m_Level = 0;
    m_VisualID = 0;
}

SpellTemplate::~SpellTemplate()
{
}

uint16 SpellTemplate::GetID() const
{
    return m_ID;
}

uint8 SpellTemplate::GetLevel() const
{
    return m_Level;
}

int16 SpellTemplate::GetVisualID() const
{
    return m_VisualID;
}

int16 SpellTemplate::GetVisualIDTarget() const
{
    return m_VisualIDTarget;
}

uint32 SpellTemplate::GetCooldown() const
{
    return m_Cooldown;
}

std::vector<SpellEffect*>* SpellTemplate::GetListEffect()
{
    return &m_ListEffect;
}

std::vector<ResourceNeed>* SpellTemplate::GetReousrcesNeed()
{
    return &m_ListResourceNeed;
}

void SpellTemplate::SetLevel(uint8 p_Level)
{
    m_Level = p_Level;
}
void SpellTemplate::SetVisualsID(int16 p_VisualID, int16 p_VisualIDTarget)
{
    m_VisualID = p_VisualID;
    m_VisualIDTarget = p_VisualIDTarget;
}

void SpellTemplate::SetCooldown(uint32 p_Cooldown)
{
    m_Cooldown = p_Cooldown;
}

void SpellTemplate::SetCastTime(uint16 p_CastTime)
{
    m_CastTime = p_CastTime;
}

void SpellTemplate::SetSpeed(float p_Speed)
{
    m_Speed = p_Speed;
}

void SpellTemplate::AddSpellEffect(SpellEffect* p_SpellEffect)
{
    m_ListEffect.push_back(p_SpellEffect);
}

void SpellTemplate::AddResourceNeed(ResourceNeed p_ResourceNeed)
{
    m_ListResourceNeed.push_back(p_ResourceNeed);
}