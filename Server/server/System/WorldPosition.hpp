#pragma once
#include "../Define.hpp"

class WorldPosition
{
public:
    WorldPosition(uint32, uint32, uint16, Orientation);
    WorldPosition();
    ~WorldPosition();

    uint32 GetPosX() const;
    uint32 GetPosY() const;
    Position GetPosition() const;
    uint16 GetMapID() const;
    Orientation GetOrientation() const;

    void SetPosX(const uint32 &);
    void SetPosY(const uint32 &);
    void SetPosition(const Position&);
    void SetMapID(const uint16 &);
    void SetOrientation(const Orientation &);

private:
    Position    m_Position;
    uint16      m_MapID;
    Orientation m_Orientation;
};

