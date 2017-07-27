#pragma once
#include "Unit.hpp"

class AnimationUnit : public Unit
{
public:
    AnimationUnit(uint16, AnimationUnitTemplate*, uint16, uint32, uint32);
    ~AnimationUnit();

    void Update(sf::Time);
    void LaunchAnim();
    void UpdateOrientation(Orientation);

private:
    uint16 m_Entry;
    uint64 m_DiffMovementTime;
    AnimationUnitTemplate* m_AnimationUnitTemplate;
    uint64 m_DiffPlayAnim;
    uint64 m_Diff;
    float m_RandMovementTime;
};

