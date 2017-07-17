#pragma once

#include "../../Entities/Unit.hpp"
#include "../../Define.hpp"

class AuraEffect
{
public:
    AuraEffect(SpellTemplate*, Unit*, Unit*, eTypeAuraEffect, const int32 &, const int32 &, const int32 &);
    ~AuraEffect();
    eTypeAuraEffect GetType() const;
    int32 GetAmount();
    Unit* GetTarget() const;
    Unit* GetCaster() const;
    void Update(const uint64 &, const uint64 &);
    void Tick();

private:
    SpellTemplate* m_SpellTemplate;
    eTypeAuraEffect m_AuraEffectType;
    int32 m_Data0;
    int32 m_Data1;
    int32 m_Data2;
    Unit* m_Target;
    Unit* m_Caster;
};