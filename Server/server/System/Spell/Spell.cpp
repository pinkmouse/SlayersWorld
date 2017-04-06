#include "Spell.hpp"
#include "../../World/PacketDefine.hpp"
#include "../../Entities/Player.hpp"
#include "../../World/WorldSocket.hpp"
#include "../../Map/Map.hpp"


Spell::Spell(SpellTemplate* p_SpellTemplate) :
    m_SpellTemplate(p_SpellTemplate)
{
    m_CastTimer = 0;
    m_SpellEffectsMap[SpellEffectType::Damage] = &Spell::EffectDamage;
    m_SpellEffectsMap[SpellEffectType::Heal] = &Spell::EffectHeal;
}

Spell::~Spell()
{
}

SpellTemplate* Spell::GetTemplate() const
{
    return m_SpellTemplate;
}

bool Spell::Prepare(Unit* p_Caster)
{
    m_Caster = p_Caster;

    /// Check Cooldown
    if (p_Caster->HasSpellCooldown(m_SpellTemplate->GetID()))
    {
        if (Player* l_Player = p_Caster->ToPlayer())
        {
            PacketWarningMsg l_Packet;
            l_Packet.BuildPacket(eTypeWarningMsg::Red, "Tant de recharge restant");
            l_Player->GetSession()->send(l_Packet.m_Packet);
        }
        return false;
    }

    /// Check Resources
    std::vector<ResourceNeed>* l_ResourcesNeed = m_SpellTemplate->GetReousrcesNeed();
    for (std::vector<ResourceNeed>::iterator l_It = l_ResourcesNeed->begin(); l_It != l_ResourcesNeed->end(); ++l_It)
    {
        ResourceNeed l_ResourceNedd = (*l_It);
        if (p_Caster->GetResourceNb(l_ResourceNedd.m_ResourceType) < l_ResourceNedd.m_Nb)
        {
            if (Player* l_Player = p_Caster->ToPlayer())
            {
                PacketWarningMsg l_Packet;
                l_Packet.BuildPacket(eTypeWarningMsg::Red, "Resources insuffisantes");
                l_Player->GetSession()->send(l_Packet.m_Packet);
                /// TODO
            }
            return false;
        }
    }

    /// ADD CAST TIME
    SetCastTime((uint64)m_SpellTemplate->GetCastTime() * 1000);
    return true;
}

void Spell::EffectDamage(Unit* p_Target, SpellEffect* p_SpellEffect)
{
    DamageInfo l_DamageInfo;
    l_DamageInfo.m_Damage = p_SpellEffect->m_BasePoint1;

    int8 l_LevelDiff = m_Caster->GetLevel() - p_Target->GetLevel();
    if (l_LevelDiff > 0)
        l_DamageInfo.m_Damage += (l_LevelDiff * 2);
    else
        l_DamageInfo.m_Damage -= (l_LevelDiff * -2);

    m_Caster->DealDamage(p_Target, l_DamageInfo);
}

void Spell::SetCastTime(uint64 p_CastTime)
{
    m_CastTimer = p_CastTime;
}

uint64 Spell::GetCastTime() const
{
    return m_CastTimer;
}

bool Spell::IsReadyToLaunch() const
{
    return m_CastTimer == 0;
}

void Spell::EffectHeal(Unit* p_Target, SpellEffect* p_SpellEffect)
{
    DamageInfo l_DamageInfo;
    l_DamageInfo.m_Damage = p_SpellEffect->m_BasePoint1;

    int8 l_LevelDiff = m_SpellTemplate->GetLevel() - p_Target->GetLevel();
    if (l_LevelDiff > 0)
        l_DamageInfo.m_Damage += (l_LevelDiff * 2);
    else
        l_DamageInfo.m_Damage -= (l_LevelDiff * -2);

    l_DamageInfo.m_Damage *= -1;
    m_Caster->DealHeal(p_Target, l_DamageInfo);
}

void Spell::LaunchEffects()
{
    if (m_Caster == nullptr)
        return;

    /// Consume Resources
    std::vector<ResourceNeed>* l_ResourcesNeed = m_SpellTemplate->GetReousrcesNeed();
    for (std::vector<ResourceNeed>::iterator l_It = l_ResourcesNeed->begin(); l_It != l_ResourcesNeed->end(); ++l_It)
    {
        ResourceNeed l_ResourceNedd = (*l_It);
        m_Caster->AddResourceNb(l_ResourceNedd.m_ResourceType, -l_ResourceNedd.m_Nb);
    }

    /// Consume Cooldown
    m_Caster->AddSpellCooldown(m_SpellTemplate->GetID(), m_SpellTemplate->GetCooldown() * 1000 /* MILLISECOND TO MICROSECOND*/);

    std::vector<SpellEffect*>* l_SpellEffects = m_SpellTemplate->GetListEffect();
    for (std::vector<SpellEffect*>::iterator l_It = l_SpellEffects->begin(); l_It != l_SpellEffects->end(); ++l_It)
    {
        SpellEffect* l_SpellEffect = (*l_It);
        std::vector<Unit*> l_Targets = SearchTargets(l_SpellEffect->m_Target, l_SpellEffect->m_RadiusMax, l_SpellEffect->m_RadiusMin);

        m_Func l_Fun = m_SpellEffectsMap[l_SpellEffect->m_EffectID];
        if (l_Fun != nullptr)
        {
            for (uint8 i = 0; i < l_Targets.size(); ++i)
            {
                /// Start Visual on target
                if (m_SpellTemplate->GetVisualIDTarget() >= 0)
                {
                    PacketUnitPlayVisual l_Packet;
                    l_Packet.BuildPacket(l_Targets[i]->GetType(), l_Targets[i]->GetID(), (uint8)m_SpellTemplate->GetVisualIDTarget());
                    l_Targets[i]->GetMap()->SendToSet(l_Packet.m_Packet, l_Targets[i]);
                }

                (this->*(l_Fun))(l_Targets[i], l_SpellEffect);
            }
        }
        else
            printf("Spell type: %d Unknow\n", l_SpellEffect->m_EffectID);
    }

    delete this;
}

std::vector<Unit*> Spell::SearchTargets(SpellTarget p_TargetType, float p_RadiusMax, float p_RadiusMin)
{
    if (!m_TargetList.empty())
        return m_TargetList;

    std::vector<Unit*> l_Targets;

    switch (p_TargetType)
    {
    case SpellTarget::Caster:
        l_Targets.push_back(m_Caster);
        break;
    case SpellTarget::CloserEnemy:
    {
        Unit* l_Target = m_Caster->GetMap()->GetCloserUnit(m_Caster, p_RadiusMax, true, true, true);
        if (l_Target != nullptr)
            l_Targets.push_back(l_Target);
        break;
    }
    case SpellTarget::ZoneEnemy:
    {
        l_Targets = m_Caster->GetMap()->GetUnitsInRadius(m_Caster, p_RadiusMin, p_RadiusMax, true, true);
        break;
    }
    default:
        break;
    }
    return l_Targets;
}

void Spell::Interrupt()
{
    delete this;
}

void Spell::SetTargetList(std::vector<Unit*> p_TargetList)
{
    m_TargetList = p_TargetList;
}