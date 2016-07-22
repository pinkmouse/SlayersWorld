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

    Orientation GetActualOrientation() const;
    void SetActualOrientation(Orientation);

    void SetPosX(uint32);
    void SetPosY(uint32);

    uint32 GetPosX() const;
    uint32 GetPosY() const;

    void Update(sf::Time);

private:
    bool m_InMovement;
    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_ActualOrientation;

    uint32 m_PosX;
    uint32 m_PosY;
    uint32 m_DiffTimeUpdate;
};

