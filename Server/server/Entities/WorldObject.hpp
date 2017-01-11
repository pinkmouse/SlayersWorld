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
    uint32 GetDistanceEuclidienne(const Position &) const;
    bool IsValidOrientationToPoint(const Orientation &, const Position &) const;
    bool IsValidOrientationToPoint(const Orientation &, const WorldObject*) const;
    Orientation GetOrientationToPoint(const Position &) const;
    Orientation GetOrientationToPoint(const WorldObject*) const;
    Orientation GetOrientationToCase(const Position &) const;

private:
    Position m_Position;
};

