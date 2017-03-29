#pragma once
#include "Unit.hpp"
#include "../Define.hpp"

class Creature : public Unit
{
public:
    Creature(int32, std::string, uint8, uint8, int16, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~Creature();
};

