#pragma once
#include "../Entities/MovementHandler.hpp"

class Unit;

class MovementHandlerCreature : public MovementHandler
{
public:
    MovementHandlerCreature(Unit*,  uint8, uint8);
    ~MovementHandlerCreature();
    void StartMovement(Orientation p_Orientation);
    void StopMovement();
    void StartAttack();
    void StopAttack();

private:
    Unit* m_Owner;
};
