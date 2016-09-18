#include "WorldObject.hpp"


WorldObject::WorldObject()
{
    m_Position.x = 0;
    m_Position.y = 0;
}


WorldObject::~WorldObject()
{
}

Position WorldObject::GetPosition() const
{
    return m_Position;
}

uint32 WorldObject::GetPosX() const
{
    return m_Position.x;
}

uint32 WorldObject::GetPosY() const
{
    return m_Position.y;
}

void WorldObject::SetPosition(const Position & p_Position)
{
    m_Position = p_Position;
}

void WorldObject::SetPosX(const uint32 & p_PosX)
{
    m_Position.x = p_PosX;
}

void WorldObject::SetPosY(const uint32 & p_PosY)
{
    m_Position.y = p_PosY;
}

float WorldObject::GetDistance(const Position & p_Position) const
{
    uint32 l_X = std::max(m_Position.x, p_Position.x) - std::min(m_Position.x, p_Position.x);
    uint32 l_Y = std::max(m_Position.y, p_Position.y) - std::min(m_Position.y, p_Position.y);

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}

float WorldObject::GetDistance(const WorldObject* p_Object) const
{
    uint32 l_X = std::max(m_Position.x, p_Object->GetPosX()) - std::min(m_Position.x, p_Object->GetPosX());
    uint32 l_Y = std::max(m_Position.y, p_Object->GetPosY()) - std::min(m_Position.y, p_Object->GetPosY());

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}