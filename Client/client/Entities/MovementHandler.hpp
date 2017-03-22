#pragma once
#include <SFML/System/Clock.hpp>
#include "../Define.hpp"
#include "../World/ClockHandler.hpp"
#include <deque>

class Map;

struct MovementAction
{
    eActionType m_ActionType;
    bool m_PositionOptions;
    Position m_Pos;
    Orientation m_Orientation;
};

class MovementHandler
{
public:
    MovementHandler(uint8, uint8);
    ~MovementHandler();
    void StartMovement(Orientation p_Orientation);
    void StartMovement();
    void StopMovement();
    void StartAttack();
    void StartAttack(Orientation);
    void StopAttack();
    uint8 GetMovementPosition();
    bool CheckNextMovement(uint32, uint32);
    bool GetInColision() const;

    bool IsInMovement() const;
    bool IsInAttack() const;
    void SetMap(Map*);

    Orientation GetOrientation() const;
    void SetOrientation(Orientation);

    void SetPosX(uint32);
    void SetPosY(uint32);
    void SetSpeed(float);

    uint32 GetPosX() const;
    uint32 GetPosY() const;
    float GetPosXAtIntant();
    float GetPosYAtIntant();
    float GetSpeed() const;

    void Update(sf::Time);
    void UpdateAnimationWalk(sf::Time);
    void UpdateAnimationAttack(sf::Time);

    void AddMovementToStack(eActionType, Position, Orientation);
    void AddMovementToStack(eActionType);
    void ClearMovementStack();
    MovementAction* GetNextMovementAction();

private:
    bool IsInColision(int64, int64) const;

    bool m_InMovement;
    bool m_InAttack;
    bool m_StopAttack;
    bool m_InColision;

    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_Orientation;

    Position m_Pos;
    uint64 m_DiffTime;
    uint64 m_DiffTimeAnim;
    bool m_AnimPeriodAsc;
    uint64 m_DiffTimeAnimAttack;
    uint8 m_SizeX;
    uint8 m_SizeY;

    Map* m_Map;
    std::deque<MovementAction> m_MovementStack;
    MovementAction m_NextMovement;
};
