#pragma once
#include <algorithm>
#include "../Define.hpp"

class WorldObject
{
public:
    WorldObject();
    ~WorldObject();
    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetPosition(const Position &);
    Position GetPosition() const;
    uint32 GetPosX() const;
    uint32 GetPosY() const;
    float GetDistance(const Position &) const;
    float GetDistance(const WorldObject*) const;

private:
    Position m_Position;
};

