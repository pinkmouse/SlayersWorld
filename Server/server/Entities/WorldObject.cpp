#include "WorldObject.hpp"
#include <cmath>

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

uint16 WorldObject::GetSideDistance(const Position & p_Position) const
{
    uint32 l_X = std::max(GetPosX(), p_Position.m_X) - std::min(GetPosX(), p_Position.m_X);
    uint32 l_Y = std::max(GetPosY(), p_Position.m_Y) - std::min(GetPosY(), p_Position.m_Y);

    return ((l_X * l_X) + (l_Y * l_Y));
}


uint32 WorldObject::GetDistanceEuclidienne(const Position & p_Position) const
{
    uint32 l_X = std::max(GetPosX(), p_Position.m_X) - std::min(GetPosX(), p_Position.m_X);
    uint32 l_Y = std::max(GetPosY(), p_Position.m_Y) - std::min(GetPosY(), p_Position.m_Y);

    return ((l_X * l_X) + (l_Y * l_Y));
}

float WorldObject::GetAngle(const Position & p_Position) const
{
    float l_X = std::max(GetPosX(), p_Position.m_X) - std::min(GetPosX(), p_Position.m_X);
    float l_Y = std::max(GetPosY(), p_Position.m_Y) - std::min(GetPosY(), p_Position.m_Y);

    if (GetPosX() > p_Position.m_X && GetPosY() > p_Position.m_Y)
        return 90.0f - atan(l_X / l_Y);
    if (GetPosX() < p_Position.m_X && GetPosY() > p_Position.m_Y)
        return 90.0f - atan(l_X / l_Y) + 90.0f;
    if (GetPosX() < p_Position.m_X && GetPosY() < p_Position.m_Y)
        return 90.0f - atan(l_X / l_Y) + 180.0f;
    return 90.0f - atan(l_X / l_Y) + 270.0f;
}

Position WorldObject::GetPositionAtDistance(const float & p_Dist, float p_Angle) const
{
    uint8 l_Pos = 0;

    while (p_Angle > 90.0f)
    {
        p_Angle -= 90.0f;
        l_Pos += 1;
    }
    p_Angle = 90.0f - p_Angle; 

    float l_Y = cos(p_Angle) * p_Dist;
    if (l_Y < 0)
        l_Y *= -1;
    float l_X = sqrt((p_Dist * p_Dist) - (l_Y * l_Y));

    switch (l_Pos)
    {
    case 0:
        l_X = GetPosX() - l_X;
        l_Y = GetPosY() - l_Y;
        break;
    case 1:
        l_X = GetPosX() + l_X;
        l_Y = GetPosY() - l_Y;
        break;
    case 2:
        l_X = GetPosX() + l_X;
        l_Y = GetPosY() + l_Y;
        break;
    case 3:
        l_X = GetPosX() - l_X;
        l_Y = GetPosY() + l_Y;
        break;
    }
    Position l_Position((uint32)l_X, (uint32)l_Y);
    return l_Position;
}

float WorldObject::GetDistance(const WorldObject* p_Object) const
{
    return GetDistance(p_Object->GetPosition());
}

uint16 WorldObject::GetSideDistance(const WorldObject* p_Object) const
{
    return GetSideDistance(p_Object->GetPosition());
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

bool WorldObject::IsValidOrientationToPoint(const Orientation & p_Orientation, const Position & p_Position) const
{
    switch (p_Orientation)
    {
    case Orientation::Down:
        if (p_Position.m_Y > GetPosY())
            return true;
        break;
    case Orientation::Up:
        if (p_Position.m_Y < GetPosY())
            return true;
        break;
    case Orientation::Right:
        if (p_Position.m_X > GetPosX())
            return true;
        break;
    case Orientation::Left:
        if (p_Position.m_X < GetPosX())
            return true;
        break;
    }
    return false;
}

bool WorldObject::IsValidOrientationToPoint(const Orientation & p_Orientation, const WorldObject* p_Object) const
{
    Position l_Posistion = p_Object->GetPosition();
    return IsValidOrientationToPoint(p_Orientation, l_Posistion);
}

Orientation  WorldObject::GetOrientationToCase(const Position & p_PositionBegin, const Position & p_Position) const
{
    int32 l_X = p_PositionBegin.m_X / TILE_SIZE - p_Position.m_X;
    int32 l_Y = p_PositionBegin.m_Y / TILE_SIZE - p_Position.m_Y;

    if (l_X < 0)
        l_X *= -1;
    if (l_Y < 0)
        l_Y *= -1;

    if (l_X > l_Y)
    {
        if (p_PositionBegin.m_X / TILE_SIZE < p_Position.m_X)
            return Orientation::Right;
        else
            return Orientation::Left;
    }
    else
    {
        if (p_PositionBegin.m_Y / TILE_SIZE < p_Position.m_Y)
            return Orientation::Down;
        else
            return Orientation::Up;
    }
}

Orientation  WorldObject::GetOrientationToCase(const Position & p_Position) const
{
    int32 l_X = GetPosX() / TILE_SIZE - p_Position.m_X;
    int32 l_Y = GetPosY() / TILE_SIZE - p_Position.m_Y;

    if (l_X < 0)
        l_X *= -1;
    if (l_Y < 0)
        l_Y *= -1;

    if (l_X > l_Y)
    {
        if (GetPosX() / TILE_SIZE < p_Position.m_X)
            return Orientation::Right;
        else
            return Orientation::Left;
    }
    else
    {
        if (GetPosY() / TILE_SIZE < p_Position.m_Y)
            return Orientation::Down;
        else
            return Orientation::Up;
    }
}

bool WorldObject::IsInCase(const Position & p_Position) const
{
    if (GetPosX() / TILE_SIZE == p_Position.m_X && GetPosY() / TILE_SIZE == p_Position.m_Y)
        return true;
    return false;
}