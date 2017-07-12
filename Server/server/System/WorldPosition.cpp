#include "WorldPosition.hpp"


WorldPosition::WorldPosition()
{
    m_MapID = 0;
    m_InstanceID = 0;
    m_Orientation = Orientation::Down;
    m_Position.m_X = 0;
    m_Position.m_Y = 0;
}

WorldPosition::WorldPosition(uint32 p_PosX, uint32 p_PosY, uint16 p_MapID, uint16 p_InstanceID, Orientation p_Orientation) :
    m_MapID(p_MapID),
    m_InstanceID(p_InstanceID),
    m_Orientation(p_Orientation)
{
    m_Position.m_X = p_PosX;
    m_Position.m_Y = p_PosY;
}

WorldPosition::~WorldPosition()
{
}

uint32 WorldPosition::GetPosX() const
{
    return m_Position.m_X;
}

uint32 WorldPosition::GetPosY() const
{
    return m_Position.m_Y;
}

Position WorldPosition::GetPosition() const
{
    return m_Position;
}

uint16 WorldPosition::GetMapID() const
{
    return m_MapID;
}

uint16 WorldPosition::GetInstanceID() const
{
    return m_InstanceID;
}

Orientation WorldPosition::GetOrientation() const
{
    return m_Orientation;
}

void WorldPosition::SetPosX(const uint32 & p_PosX)
{
    m_Position.m_X = p_PosX;
}

void WorldPosition::SetPosY(const uint32 & p_PosY)
{
    m_Position.m_Y = p_PosY;
}

void WorldPosition::SetPosition(const Position & p_Position)
{
    m_Position = p_Position;
}

void WorldPosition::SetMapID(const uint16 & p_MapID)
{
    m_MapID = p_MapID;
}

void WorldPosition::SetInstanceID(const uint16 & p_InstanceID)
{
    m_InstanceID = p_InstanceID;
}

void WorldPosition::SetOrientation(const Orientation & p_Orientation)
{
    m_Orientation = p_Orientation;
}