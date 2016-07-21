#pragma once
#include <SFML/System/Clock.hpp>
#include "../../Define.hpp"

class Movement
{
public:
    Movement();
    ~Movement();
    void StartMovement();
    void StopMovement();
    uint8 GetMovementPosition();
    bool IsInMovement() const;

    Orientation GetActualOrientation() const;
    void SetActualOrientation(Orientation);

private:
    bool m_InMovement;
    sf::Clock* m_Clock;
    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_ActualOrientation;
};

