#pragma once
#include <algorithm>
#include "../System/WorldPosition.hpp"
#include "../Define.hpp"

class WorldObject : public WorldPosition
{
public:
    WorldObject();
    ~WorldObject();
    float GetDistance(const Position &) const;
    float GetDistance(const WorldObject*) const;
    Orientation GetOrientationToPoint(const Position &) const;
    Orientation GetOrientationToPoint(const WorldObject*) const;

private:
    Position m_Position;
};

