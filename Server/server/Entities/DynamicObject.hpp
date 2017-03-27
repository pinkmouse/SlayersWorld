#pragma once
#include "WorldObject.hpp"
#include "../Define.hpp"
#include "Unit.hpp"
#include <SFML/System/Time.hpp>

class Map;
class DynamicObject : public WorldObject
{
public:
    DynamicObject(Map*, uint32, uint32);
    ~DynamicObject();
    virtual void Update(sf::Time) = 0;
    void SetInWorld(bool);
    bool IsInWorld() const;
    Map* GetMap() const;
    virtual void UnitEnterInCase(Unit*) = 0;
    virtual bool CanBeWalk() = 0;
    virtual bool IsBlocking() = 0;

private:
    Map* m_Map;
    bool m_InWorld;
};

