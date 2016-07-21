#include "Movement.hpp"



Movement::Movement()
{
    m_Clock = new sf::Clock();
    m_Speed = 1.0f;
    m_InMovement = false;
    m_MovementPosition = 0;
    m_ActualOrientation = Orientation::Down;
}


Movement::~Movement()
{
}

bool Movement::IsInMovement() const
{
    return m_InMovement;
}

void Movement::StartMovement()
{
    m_InMovement = true;
    m_Clock->restart();
}

void Movement::StopMovement()
{
    m_InMovement = false;
}

void Movement::SetActualOrientation(Orientation p_Orientation)
{
    m_ActualOrientation = p_Orientation;
}

Orientation Movement::GetActualOrientation() const
{
    return m_ActualOrientation;
}


uint8 Movement::GetMovementPosition()
{
    if (!m_InMovement)
        return 0;

    if (m_Clock->getElapsedTime().asMilliseconds() > UPDATE_TIME_MOVEMENT * m_Speed)
    {
        m_Clock->restart();
        m_MovementPosition++;
        if (m_MovementPosition >= MAX_MOVEMENT_POSITION)
            m_MovementPosition = 0;
    }
    return m_MovementPosition;
}