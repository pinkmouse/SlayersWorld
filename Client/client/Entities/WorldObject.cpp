#include "WorldObject.hpp"


WorldObject::WorldObject(TypeWorldObject p_Type, uint8 p_SizeX, uint8 p_SizeY) :
    m_Type(p_Type),
    m_SizeX(p_SizeX),
    m_SizeY(p_SizeY),
    m_Tile(nullptr)
{
}


WorldObject::~WorldObject()
{
}

TypeWorldObject WorldObject::GetType() const
{
    return m_Type;
}

sf::Sprite* WorldObject::GetSprite() const
{
    return m_Tile;
}

void WorldObject::SetSprite(sf::Sprite* p_Tile)
{
    m_Tile = p_Tile;
}

float WorldObject::GetDistance(const Position & p_Position) const
{
    uint32 l_X = std::max(GetPosX(), p_Position.x) - std::min(GetPosX(), p_Position.x);
    uint32 l_Y = std::max(GetPosY(), p_Position.y) - std::min(GetPosY(), p_Position.y);

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}

float WorldObject::GetDistance(const WorldObject* p_Object) const
{
    uint32 l_X = std::max(GetPosX(), p_Object->GetPosX()) - std::min(GetPosX(), p_Object->GetPosX());
    uint32 l_Y = std::max(GetPosY(), p_Object->GetPosY()) - std::min(GetPosY(), p_Object->GetPosY());

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}

Unit* WorldObject::ToUnit()
{
    if (m_Type == TypeWorldObject::UNIT)
        return  reinterpret_cast<Unit*>(this);
    else
        return nullptr;
}

uint8 WorldObject::GetSizeX() const
{
    return m_SizeX;
}

uint8 WorldObject::GetSizeY() const
{
    return m_SizeY;
}