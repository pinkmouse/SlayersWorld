#pragma once
#include "../../Define.hpp"

class Spell
{
public:
    Spell(uint16);
    ~Spell();

    uint16 GetID();
    uint8 GetLevel();
    uint16 GetVisualID();
    std::vector<SpellEffect*>* GetListEffect();
    void SetLevel(uint8);
    void SetVisualID(uint16);
    void AddSpellEffect(SpellEffect*);

private:
    uint16 m_ID;
    uint8 m_Level;
    uint16 m_VisualID;
    std::vector<SpellEffect*> m_ListEffect;
};