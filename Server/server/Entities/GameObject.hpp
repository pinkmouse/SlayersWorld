#pragma once
#include "DynamicObject.hpp"

class GameObject : public DynamicObject
{
public:
    GameObject(Map*, uint32, uint32);
    ~GameObject();
    void Update(sf::Time);
};

