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

int16 SpellTemplate::GetVisualIDUnder() const
{
    return m_VisualIDUnder;
}

int16 SpellTemplate::GetVisualIDTargetUnder() const
{
    return m_VisualIDTargetUnder;
}

uint32 SpellTemplate::GetCooldown() const
{
    return m_Cooldown;
}

int32 SpellTemplate::GetDuration() const
{
    return m_Duration;
}

uint16 SpellTemplate::GetCastTime() const
{
    return m_CastTime;
}

std::string SpellTemplate::GetName() const
{
    return m_Name;
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

bool SpellTemplate::HasAuraEffect(const eTypeAuraEffect & p_AuraEffect)
{
    for (auto l_Effect : m_ListEffect)
    {
        if (l_Effect->m_EffectID == SpellEffectType::ApplyAura && l_Effect->m_BasePoint1 == p_AuraEffect)
            return true;
    }
    return false;
}

void SpellTemplate::SetVisualsID(const int16 & p_VisualIDUnder, const int16 & p_VisualID, const int16 & p_VisualIDTargetUnder, const int16 & p_VisualIDTarget)
{
    m_VisualIDUnder = p_VisualIDUnder;
    m_VisualID = p_VisualID;
    m_VisualIDTargetUnder = p_VisualIDTargetUnder;
    m_VisualIDTarget = p_VisualIDTarget;
}

void SpellTemplate::SetCooldown(uint32 p_Cooldown)
{
    m_Cooldown = p_Cooldown;
}

void SpellTemplate::SetDuration(int32 p_Duration)
{
    m_Duration = p_Duration;
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

void SpellTemplate::SetName(const std::string & p_Name)
{
    m_Name = p_Name;
}