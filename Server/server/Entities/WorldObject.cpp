#include "WorldObject.hpp"


WorldObject::WorldObject()
{
}


WorldObject::~WorldObject()
{
}

float WorldObject::GetDistance(const Position & p_Position) const
{
    uint32 l_X = std::max(GetPosX(), p_Position.m_X) - std::min(GetPosX(), p_Position.m_X);
    uint32 l_Y = std::max(GetPosY(), p_Position.m_Y) - std::min(GetPosY(), p_Position.m_Y);

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}

float WorldObject::GetDistance(const WorldObject* p_Object) const
{
    return GetDistance(p_Object->GetPosition());
}


Orientation  WorldObject::GetOrientationToPoint(const Position & p_Position) const
{
    int32 l_X = GetPosX() - p_Position.m_X;
    int32 l_Y = GetPosY() - p_Position.m_Y;

    if (l_X < 0)
        l_X *= -1;
    if (l_Y < 0)
        l_Y *= -1;

    if (l_X > l_Y)
    {
        if (GetPosX() < p_Position.m_X)
            return Orientation::Right;
        else
            return Orientation::Left;
    }
    else
    {
        if (GetPosY() < p_Position.m_Y)
            return Orientation::Down;
        else
            return Orientation::Up;
    }
}

Orientation  WorldObject::GetOrientationToPoint(const WorldObject* p_Object) const
{
    Position l_Posistion = p_Object->GetPosition();
    return GetOrientationToPoint(l_Posistion);
}