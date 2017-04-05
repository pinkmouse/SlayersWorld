#pragma once
#include "WorldObject.hpp"
#include "../Define.hpp"
#include "Unit.hpp"
#include <SFML/System/Time.hpp>

class DynamicObject : public Unit
{
public:
    DynamicObject(int32, TypeUnit, std::string, uint8, uint8, int16, uint8, uint8, uint16, uint32, uint32, Orientation, bool);
    ~DynamicObject();
    bool IsBlocking();

private:
    bool m_Blocking;
};

