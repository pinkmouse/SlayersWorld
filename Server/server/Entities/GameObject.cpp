#include "GameObject.hpp"


GameObject::GameObject(Map* p_Map, uint32 p_PosX, uint32 p_PosY, int16 p_Skin, bool p_Blocking) :
    DynamicObject(0, 0, TypeUnit::GAMEOBJECT, p_Map, p_PosX, p_PosY, p_Skin),
    m_Blocking(p_Blocking)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update(sf::Time m_Diff)
{
}

bool GameObject::IsBlocking()
{
    return false;
}
