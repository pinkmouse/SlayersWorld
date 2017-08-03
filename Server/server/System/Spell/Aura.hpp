#pragma once
#include "../../Define.hpp"
#include "../../Entities/Unit.hpp"
#include "AuraEffect.hpp"
#include "SpellTemplate.hpp"


class Aura
{
public:
    Aura(Unit*, Unit*, SpellTemplate*);
    ~Aura();
    AuraEffect* AddAuraEffect(const uint8 &, const eTypeAuraEffect &, const int32 &, const int32 & ,const int32 &);
    int64 GetDuration() const;
    int64 GetMaxDuration() const;
    void Update(sf::Time);
    void SetDuration(const uint64 &, bool p_Natual = false);
    void AddDuration(const uint64 &);
    void SubDuration(const uint64 &);
    SpellTemplate* GetSpellTemplate() const;
    std::vector<AuraEffect*> GetAuraEffectType(const eTypeAuraEffect &);
    Unit* GetCaster() const;
    TypeUnit GetCasterType() const;
    uint16 GetCasterID() const;
    bool HasEffect(const eTypeAuraEffect &);

    /// AURA EFFECTS
    void AuraEffectUpdateSpeed(AuraEffect*, bool);
    void AuraEffectPeriodicHeal(AuraEffect*, bool);
    void AuraEffectPeriodicDamage(AuraEffect*, bool);
    void AuraEffectModifyDamagePct(AuraEffect*, bool);
    void AuraEffectMount(AuraEffect*, bool);
    
private:
    uint16  m_ID;
    Unit* m_Caster;
    Unit* m_Target;
    uint64  m_Duration;
    SpellTemplate* m_SpellTemplate;
    std::map<uint8, AuraEffect*> m_AuraEffectList;

    typedef void(Aura::*m_AuraEffectFunc)(AuraEffect*, bool);
    std::map<eTypeAuraEffect, m_AuraEffectFunc> m_AuraEffectsMap;
    TypeUnit m_CasterType;
    uint16 m_CasterID;
};