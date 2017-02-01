#pragma once
#include "../../Define.hpp"
#include <map>
#include "Spell.hpp"

class SpellManager
{
public:
    SpellManager();
    ~SpellManager();
    Spell* GetSpell(uint16);
    void AddSpell(Spell*);
    SpellEffect* GetSpellEffect(uint16);
    void AddSpellEffect(SpellEffect);

private:
    std::map< uint16, Spell* > m_Spells;
    std::map< uint16, SpellEffect > m_SpellEffects;
};