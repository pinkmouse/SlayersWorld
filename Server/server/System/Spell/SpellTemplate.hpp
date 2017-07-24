#pragma once
#include "../../Define.hpp"

class SpellTemplate
{
public:
    SpellTemplate(uint16);
    ~SpellTemplate();

    uint16 GetID() const;
    uint8 GetLevel() const;
    int16 GetVisualID() const;
    int16 GetVisualIDTarget() const;
    uint16 GetCastTime() const;
    uint32 GetCooldown() const;
    int32 GetDuration() const;
    std::string GetName() const;
    std::vector<SpellEffect*>* GetListEffect();
    std::vector<ResourceNeed>* GetReousrcesNeed();
    void SetLevel(uint8);
    void SetVisualsID(int16, int16);
    void SetCooldown(uint32);
    void SetCastTime(uint16);
    void SetDuration(int32);
    void SetSpeed(float);
    void SetName(const std::string &);
    void AddSpellEffect(SpellEffect*);
    void AddResourceNeed(ResourceNeed);

private:
    uint16 m_ID;
    uint8 m_Level;
    int16 m_VisualID;
    int16 m_VisualIDTarget;
    uint32 m_Cooldown; ///< In Millisecond
    uint16 m_CastTime; ///< In Millisecond
    int32 m_Duration; ///< In Millisecond
    float m_Speed;
    std::string m_Name;

    std::vector<SpellEffect*> m_ListEffect;
    std::vector<ResourceNeed> m_ListResourceNeed;
};