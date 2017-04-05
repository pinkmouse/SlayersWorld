#pragma once
#include "WorldObject.hpp"
#include "../Define.hpp"
#include "Unit.hpp"
#include <SFML/System/Time.hpp>

class Map;
class DynamicObject : public Unit
{
public:
    DynamicObject(uint16, uint16, TypeUnit, Map*, uint32, uint32, int16);
    ~DynamicObject();
    virtual void Update(sf::Time) = 0;
    void Respawn();
    void SetResourceNb(eResourceType, uint8);
    virtual void ActionFrom(Player*);
    virtual void UnitEnterInCase(Unit*) = 0;
    virtual bool CanBeWalk() = 0;
    virtual bool IsBlocking() const;
};

