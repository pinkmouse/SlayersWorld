#pragma once
#include "Unit.hpp"

class Creature : public Unit
{
public:
    Creature(uint16, uint16, CreatureTemplate, uint16, uint32, uint32);
    ~Creature();

    void Update(sf::Time);
    void UpdatePassive(sf::Time);
    void UpdateDefensive(sf::Time);
    void RandMoving();
    void StartMovement(Orientation);
    void StopMovement();
    void StartAttack(Unit*);
    void StopAttack();
    void UpdateOrientation(Orientation);
    void Respawn();
    void Unspawn();
    void SetHealth(const uint8 &);

private:
    uint16 m_Entry;
    uint64 m_DiffMovementTime;
    CreatureTemplate m_CreatureTemplate;
    uint16 m_RandMovementTime;
    bool m_Evade;
};
