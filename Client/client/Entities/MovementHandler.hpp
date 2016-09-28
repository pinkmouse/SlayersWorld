#pragma once
#include <SFML/System/Clock.hpp>
#include "../Define.hpp"
#include "../World/ClockHandler.hpp"
#include <queue>

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
    bool CheckNextMovement(uint32, uint32);

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

    void Update(sf::Time);
    void UpdateAnimationWalk(sf::Time);
    void UpdateAnimationAttack(sf::Time);
    void Clear();

    void AddMovementToStack(eActionType, Position, Orientation);
    void AddMovementToStack(eActionType);

private:
    bool m_InMovement;
    bool m_InAttack;
    bool m_StopAttack;

    float m_Speed;
    uint8 m_MovementPosition;
    Orientation m_Orientation;

    Position m_Pos;
    uint64 m_DiffTime;
    uint64 m_DiffTimeAnim;
    uint64 m_DiffTimeAnimAttack;
    uint8 m_SizeX;
    uint8 m_SizeY;

    Map* m_Map;
    struct MovementAction
    {
        eActionType m_ActionType;
        bool m_PositionOptions;
        Position m_Pos;
        Orientation m_Orientation;
    };
    std::queue<MovementAction> m_MovementStack;
};
