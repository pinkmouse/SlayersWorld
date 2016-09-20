#include "WorldPosition.hpp"


WorldPosition::WorldPosition()
{
    m_Position.x = 0;
    m_Position.y = 0;
    m_MapID = 0;
    m_Orientation = Orientation::Down;
}

WorldPosition::WorldPosition(uint32 p_PosX, uint32 p_PosY, uint16 p_MapID, Orientation p_Orientation) :
    m_MapID(p_MapID),
    m_Orientation(p_Orientation)
{
    m_Position.x = p_PosX;
    m_Position.y = p_PosY;
}



WorldPosition::~WorldPosition()
{
}

uint32 WorldPosition::GetPosX() const
{
    return m_Position.x;
}

uint32 WorldPosition::GetPosY() const
{
    return m_Position.y;
}

Position WorldPosition::GetPosition() const
{
    return m_Position;
}

uint16 WorldPosition::GetMapID() const
{
    return m_MapID;
}

Orientation WorldPosition::GetOrientation() const
{
    return m_Orientation;
}

void WorldPosition::SetPosX(const uint32 & p_PosX)
{
    m_Position.x = p_PosX;
}

void WorldPosition::SetPosY(const uint32 & p_PosY)
{
    m_Position.y = p_PosY;
}

void WorldPosition::SetPosition(const Position & p_Position)
{
    m_Position = p_Position;
}

void WorldPosition::SetMapID(const uint16 & p_MapID)
{
    m_MapID = p_MapID;
}

void WorldPosition::SetOrientation(const Orientation & p_Orientation)
{
    m_Orientation = p_Orientation;
}