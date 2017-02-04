#include "Spell.hpp"
#include "../../World/PacketDefine.hpp"
#include "../../Entities/Player.hpp"
#include "../../World/WorldSocket.hpp"


Spell::Spell(SpellTemplate* p_SpellTemplate) :
    m_SpellTemplate(p_SpellTemplate)
{
}

Spell::~Spell()
{
}

bool Spell::Prepare(Unit* p_Caster)
{
    m_Caster = p_Caster;

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
                l_Packet.BuildPacket(0);
                l_Player->GetSession()->send(l_Packet.m_Packet);
                /// TODO
            }
            return false;
        }
    }

    /// Consume Resources
    for (std::vector<ResourceNeed>::iterator l_It = l_ResourcesNeed->begin(); l_It != l_ResourcesNeed->end(); ++l_It)
    {
        ResourceNeed l_ResourceNedd = (*l_It);
        p_Caster->AddResourceNb(l_ResourceNedd.m_ResourceType, -l_ResourceNedd.m_Nb);
    }

    return true;
}

void Spell::EffectDamage(Unit* p_Target, int8 l_Coeff)
{
    DamageInfo l_DamageInfo;
    l_DamageInfo.m_Damage = 20;
    m_Caster->DealDamage(p_Target, l_DamageInfo);
}

void Spell::LaunchEffects()
{
    std::vector<SpellEffect*>* l_SpellEffects = m_SpellTemplate->GetListEffect();
    for (std::vector<SpellEffect*>::iterator l_It = l_SpellEffects->begin(); l_It != l_SpellEffects->end(); ++l_It)
    {
        SpellEffect* l_SpellEffect = (*l_It);
        printf("Lauch effect : %d\n", l_SpellEffect->m_EffectID);
        /// TODO
    }
}

void Spell::SearchTargets()
{
}