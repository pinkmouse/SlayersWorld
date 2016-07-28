#pragma once
#include "Unit.hpp"

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~Player();

private:
};

