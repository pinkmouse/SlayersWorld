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

private:
    Position m_Position;
};

