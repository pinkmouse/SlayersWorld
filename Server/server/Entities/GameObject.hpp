#pragma once
#include "DynamicObject.hpp"

class GameObject : public DynamicObject
{
public:
    GameObject(Map*, uint32, uint32, uint8, bool);
    ~GameObject();
    void Update(sf::Time);
    bool IsBlocking();

private:
    bool m_Blocking;
    uint8 m_Skin;
};

