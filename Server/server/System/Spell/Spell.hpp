#pragma once
#include "../../Define.hpp"
#include "SpellTemplate.hpp"
#include "AuraEffect.hpp"
#include "../../Entities/Unit.hpp"


class Spell
{
public:
    Spell(SpellTemplate*);
    ~Spell();

    bool Prepare(Unit*);
    void LaunchEffects();

    std::vector<Unit*> SearchTargets(SpellTarget, float, float p_RadiusMin = 0.0f);

    /// EFFECTS
    void EffectDamage(uint8, Unit*, SpellEffect*);
    void EffectHeal(uint8, Unit*, SpellEffect*);
    void EffectTeleport(uint8, Unit*, SpellEffect*);
    void EffectApplyAura(uint8, Unit*, SpellEffect*);
    void EffectLearnClass(uint8, Unit*, SpellEffect*);
    void EffectLearnSpell(uint8, Unit*, SpellEffect*);

    void SetCastTime(uint64);
    uint64 GetCastTime() const;
    bool IsReadyToLaunch() const;
    SpellTemplate* GetTemplate() const;
    void Interrupt();
    void SetTargetList(std::vector<Unit*>);

private:
    SpellTemplate* m_SpellTemplate;
    Unit* m_Caster;
    uint64 m_CastTimer;

    typedef void(Spell::*m_SpellFunc)(uint8, Unit*, SpellEffect*);

    std::map<SpellEffectType, m_SpellFunc> m_SpellEffectsMap;
    std::vector<Unit*> m_TargetList;
};