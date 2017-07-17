#include "AuraEffect.hpp"
#include "../../Define.hpp"


AuraEffect::AuraEffect(SpellTemplate* p_SpellTemplate, Unit* p_Caster, Unit* p_Target, eTypeAuraEffect p_Type, const int32 & p_Data0, const int32 & p_Data1, const int32 & p_Data2) :
    m_SpellTemplate(p_SpellTemplate),
    m_Caster(p_Caster),
    m_Target(p_Target),
    m_AuraEffectType(p_Type),
    m_Data0(p_Data0),
    m_Data1(p_Data1),
    m_Data2(p_Data2)
{
    printf("---> AddAura %d With amount : %d\n", (int32)p_Type, p_Data0);
}

AuraEffect::~AuraEffect()
{
}

void AuraEffect::Tick()
{
    switch (m_AuraEffectType)
    {
    case eTypeAuraEffect::PERIODIC_DAMAGE:
    {
        DamageInfo l_DamageInfo = m_Caster->CalculSpellDamageToTarget(m_Target, m_Data0, m_SpellTemplate->GetLevel());
        m_Caster->DealDamage(m_Target, l_DamageInfo);
    }
    case eTypeAuraEffect::PERIODIC_HEAL:
    {
        DamageInfo l_DamageInfo = m_Caster->CalculHealToTarget(m_Target, m_Data0, m_SpellTemplate->GetLevel());
        m_Caster->DealHeal(m_Target, l_DamageInfo);
        break;
    }
    default:
        break;
    }
}

void AuraEffect::Update(const uint64 & p_OldDuration, const uint64 & p_NewDuration)
{
    switch (m_AuraEffectType)
    {
        case eTypeAuraEffect::PERIODIC_DAMAGE:
        case eTypeAuraEffect::PERIODIC_HEAL:
        {
            if (p_OldDuration / ((uint64)m_Data1 * 1000) != (p_NewDuration / ((uint64)m_Data1 * 1000)))
                Tick();
            break;
        }
        default: 
            break;
    }
}

eTypeAuraEffect AuraEffect::GetType() const
{
    return m_AuraEffectType;
}

int32 AuraEffect::GetAmount()
{
    return m_Data0;
}

Unit* AuraEffect::GetTarget() const
{
    return m_Target;
}

Unit* AuraEffect::GetCaster() const
{
    return m_Caster;
}