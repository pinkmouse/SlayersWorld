#pragma once
#include "Unit.hpp"

class Creature : public Unit
{
public:
    Creature(uint16, uint16, CreatureTemplate, uint16, uint32, uint32);
    ~Creature();
    void Update(sf::Time);

private:
    uint16 m_Entry;
    uint64 m_DiffMovementTime;
    CreatureTemplate m_CreatureTemplate;
};

