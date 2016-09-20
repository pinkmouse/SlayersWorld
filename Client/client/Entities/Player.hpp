#pragma once
#include "Unit.hpp"
#include "../Define.hpp"

class Player : public Unit
{
public:
    Player(int32, std::string, uint8, uint8, uint8, uint16, uint32, uint32, Orientation);
    ~Player();
    void SetAlignment(const uint8 &);
    uint8 GetAlignment() const;

private:
    uint8 m_Alignment;
};

