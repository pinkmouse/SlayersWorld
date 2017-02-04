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

    std::vector<Unit*> SearchTargets(SpellTarget);
    void EffectDamage(Unit*, SpellEffect*);

private:
    SpellTemplate* m_SpellTemplate;
    Unit* m_Caster;

    typedef void(Spell::*m_Func)(Unit*, SpellEffect*);
    std::map<SpellEffectType, m_Func> m_SpellEffectsMap;
};