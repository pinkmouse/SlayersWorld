#pragma once
#include "DynamicObject.hpp"

class GameObject : public DynamicObject
{
public:
    GameObject(Case*);
    ~GameObject();
    void Update(sf::Time);
};

