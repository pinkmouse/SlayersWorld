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

    void SearchTargets();

private:
    SpellTemplate* m_SpellTemplate;
    Unit* m_Caster;
    std::vector<Unit*> m_Targets;
};