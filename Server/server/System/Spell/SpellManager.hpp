#pragma once
#include "../../Define.hpp"
#include <map>
#include "SpellTemplate.hpp"

class SpellManager
{
public:
    SpellManager();
    ~SpellManager();
    SpellTemplate* GetSpell(uint16);
    void AddSpell(SpellTemplate*);
    SpellEffect* GetSpellEffect(uint16);
    void AddSpellEffect(SpellEffect);

private:
    std::map< uint16, SpellTemplate* > m_Spells;
    std::map< uint16, SpellEffect > m_SpellEffects;
};