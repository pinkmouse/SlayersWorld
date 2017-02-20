#pragma once
#include "WorldObject.hpp"
#include "../Define.hpp"
#include "../Map/Case.hpp"
#include <SFML/System/Time.hpp>

class DynamicObject : public WorldObject
{
public:
    DynamicObject(Case*);
    ~DynamicObject();
    virtual void Update(sf::Time) = 0;
    void SetInWorld(bool);
    bool IsInWorld() const;

private:
    Case* m_Case;
    bool m_InWorld;
};

