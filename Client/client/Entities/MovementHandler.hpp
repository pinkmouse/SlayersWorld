#pragma once
#include <SFML/System/Clock.hpp>
#include "../Define.hpp"
#include "../World/ClockHandler.hpp"

class MovementHandler
{
public:
    MovementHandler();
    ~MovementHandler();
    void StartMovement(Orientation p_Orientation);
    void StopMovement();
    uint8 GetMovementPosition();
    bool IsInMovement() const;

    Orientation GetOrientation() const;
    void SetOrientation(Orientation);

    void SetPosX(uint32);
    void SetPosY(uint32);

    uint32 GetPosX() const;
    uint32 GetPosY() const;

    void Update(sf::Time);
    void UpdateAnimationWalk(sf::Time);

private:
    bool m_InMovement;
    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_Orientation;

    uint32 m_PosX;
    uint32 m_PosY;
    uint64 m_DiffTime;
    uint64 m_DiffTimeUpdateAnimation;
};