#pragma once
#include <algorithm>
#include "../System/WorldPosition.hpp"
#include "../Define.hpp"

class WorldObject : public WorldPosition
{
public:
    WorldObject();
    ~WorldObject();
    bool IsInCase(const Position &) const;
    float GetDistance(const Position &) const;
    float GetDistance(const WorldObject*) const;
    uint16 GetSideDistance(const WorldObject*) const;
    uint16 GetSideDistance(const Position &) const;
    uint32 GetDistanceEuclidienne(const Position &) const;
    float GetAngle(const Position &) const;
    bool IsValidOrientationToPoint(const Orientation &, const Position &) const;
    bool IsValidOrientationToPoint(const Orientation &, const WorldObject*) const;
    Position GetPositionAtDistance(const float &, float) const;
    Orientation GetOrientationToPoint(const Position &) const;
    Orientation GetOrientationToPoint(const WorldObject*) const;
    Orientation GetOrientationToCase(const Position &) const;
    Orientation GetOrientationToCase(const Position &, const Position &) const;

private:
    Position m_Position;
    uint16 m_InstanceID;
};

