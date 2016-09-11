#pragma once
#include <SFML/System/Clock.hpp>
#include "../Define.hpp"
#include "../World/ClockHandler.hpp"

class Map;

class MovementHandler
{
public:
    MovementHandler(uint8, uint8);
    ~MovementHandler();
    void StartMovement(Orientation p_Orientation);
    void StopMovement();
    void StopMovementAt(const uint32 &, const uint32 &);
    bool IsInMovement() const;
    void SetMap(Map*);
    bool IsInColision(int64, int64) const;

    Orientation GetOrientation() const;
    void SetOrientation(Orientation);

    void SetPosX(uint32);
    void SetPosY(uint32);

    Position GetPos() const;
    uint32 GetPosX() const;
    uint32 GetPosY() const;

    void Update(sf::Time);

private:
    bool m_InMovement;
    float m_Speed;
    Orientation m_Orientation;
    
    Position m_Pos;
    uint64 m_DiffTime;
    uint8 m_SizeX;
    uint8 m_SizeY;

    Map* m_Map;

    struct StopPos
    {
        Position Pos;
        bool Actif;
    };
    StopPos m_StopPos;

};
