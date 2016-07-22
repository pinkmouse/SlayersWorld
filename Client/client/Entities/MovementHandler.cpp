#include "MovementHandler.hpp"



MovementHandler::MovementHandler()
{
    m_Speed = 1.0f;
    m_InMovement = false;
    m_MovementPosition = 0;
    m_ActualOrientation = Orientation::Down;
    m_PosX = 0;
    m_PosY = 0;
    m_DiffTimeUpdate = 0;
}


MovementHandler::~MovementHandler()
{
}

void MovementHandler::Update(sf::Time p_Diff)
{
    if (m_DiffTimeUpdate > UPDATE_TIME_MOVEMENT * m_Speed)
    {
        m_MovementPosition++;
        if (m_MovementPosition >= MAX_MOVEMENT_POSITION)
            m_MovementPosition = 0;
        m_DiffTimeUpdate -= UPDATE_TIME_MOVEMENT * m_Speed;
    }
    else
        m_DiffTimeUpdate += p_Diff.asMilliseconds();
}

bool MovementHandler::IsInMovement() const
{
    return m_InMovement;
}

void MovementHandler::StartMovement(Orientation p_Orientation)
{
    m_InMovement = true;
    m_MovementPosition = 2;
}

void MovementHandler::StopMovement()
{
    m_InMovement = false;
    m_MovementPosition = 1;
}

void MovementHandler::SetActualOrientation(Orientation p_Orientation)
{
    m_ActualOrientation = p_Orientation;
}

Orientation MovementHandler::GetActualOrientation() const
{
    return m_ActualOrientation;
}


uint8 MovementHandler::GetMovementPosition()
{
    if (!m_InMovement)
        return m_MovementPosition;

    return m_MovementPosition;
}

void MovementHandler::SetPosX(uint32 p_PosX)
{
    m_PosX = p_PosX;
}

void MovementHandler::SetPosY(uint32 p_PosY)
{
    m_PosY = p_PosY;
}

uint32 MovementHandler::GetPosX() const
{
    return m_PosX;
}

uint32 MovementHandler::GetPosY() const
{
    return m_PosY;
}