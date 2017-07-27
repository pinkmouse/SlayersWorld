#pragma once
#include "Unit.hpp"
#include "../Define.hpp"

class AnimationUnit : public Unit
{
public:
    AnimationUnit(int32, std::string, uint8, uint8, int16, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~AnimationUnit();
    void LaunchAnim();
    bool IsInAnimation() const;
    uint8 GetFrame() const;
    void Update(sf::Time);

private:
    bool m_IsInAnimation;
    uint8 m_FrameAnim;
    uint64 m_DiffAnim;
};

