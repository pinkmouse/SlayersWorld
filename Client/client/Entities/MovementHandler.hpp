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
    void StartMovement();
    void StopMovement();
    void StartAttack();
    void StopAttack();
    uint8 GetMovementPosition();
    bool IsInColision(int64, int64) const;
    bool IsInMovement() const;
    bool IsInAttack() const;
    void SetMap(Map*);

    Orientation GetOrientation() const;
    void SetOrientation(Orientation);

    void SetPosX(uint32);
    void SetPosY(uint32);

    uint32 GetPosX() const;
    uint32 GetPosY() const;

    void Update(sf::Time);
    void UpdateAnimationWalk(sf::Time);
    void UpdateAnimationAttack(sf::Time);

private:
    bool m_InMovement;
    bool m_InAttack;
    bool m_StopAttack;

    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_Orientation;

    uint32 m_PosX;
    uint32 m_PosY;
    uint64 m_DiffTime;
    uint64 m_DiffTimeAnim;
    uint64 m_DiffTimeAnimAttack;
    uint8 m_SizeX;
    uint8 m_SizeY;

    Map* m_Map;
};
