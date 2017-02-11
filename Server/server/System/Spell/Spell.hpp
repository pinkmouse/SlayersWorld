#pragma once
#include "../../Define.hpp"
#include "SpellTemplate.hpp"
#include "../../Entities/Unit.hpp"


class Spell
{
public:
    Spell(SpellTemplate*);
    ~Spell();

    bool Prepare(Unit*);
    void LaunchEffects();

    std::vector<Unit*> SearchTargets(SpellTarget, float, float p_RadiusMin = 0.0f);
    void EffectDamage(Unit*, SpellEffect*);
    void EffectHeal(Unit*, SpellEffect*);
    void SetCastTime(uint64);
    uint64 GetCastTime() const;
    bool IsReadyToLaunch() const;
    SpellTemplate* GetTemplate() const;

private:
    SpellTemplate* m_SpellTemplate;
    Unit* m_Caster;
    uint64 m_CastTimer;

    typedef void(Spell::*m_Func)(Unit*, SpellEffect*);
    std::map<SpellEffectType, m_Func> m_SpellEffectsMap;
};