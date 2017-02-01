#pragma once
#include "../../Define.hpp"

class SpellTemplate
{
public:
    SpellTemplate(uint16);
    ~SpellTemplate();

    uint16 GetID();
    uint8 GetLevel();
    uint16 GetVisualID();
    std::vector<SpellEffect*>* GetListEffect();
    std::vector<ResourceNeed>* GetReousrcesNeed();
    void SetLevel(uint8);
    void SetVisualsID(int16, int16);
    void SetCooldown(uint32);
    void SetCastTime(uint16);
    void AddSpellEffect(SpellEffect*);
    void AddResourceNeed(ResourceNeed);

private:
    uint16 m_ID;
    uint8 m_Level;
    int16 m_VisualID;
    int16 m_VisualIDTarget;
    uint32 m_Cooldown; ///< In Millisecond
    uint16 m_CastTime; ///< In Millisecond
    
    std::vector<SpellEffect*> m_ListEffect;
    std::vector<ResourceNeed> m_ListResourceNeed;
};