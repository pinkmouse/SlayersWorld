#include "GameObject.hpp"


GameObject::GameObject(Map* p_Map, uint32 p_PosX, uint32 p_PosY) :
    DynamicObject(p_Map, p_PosX, p_PosY)
{
}


GameObject::~GameObject()
{
}

void GameObject::Update(sf::Time m_Diff)
{
}