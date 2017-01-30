#pragma once
#include <algorithm>
#include "../System/WorldPosition.hpp"
#include "../Define.hpp"
#include "../Graphics/TileSprite.hpp"

class Unit;

class WorldObject : public WorldPosition
{
public:
    WorldObject(TypeWorldObject, uint8, uint8);
    ~WorldObject();
    float GetDistance(const Position &) const;
    float GetDistance(const WorldObject*) const;
    uint8 GetSizeX() const;
    uint8 GetSizeY() const;

    sf::Sprite* GetSprite() const;
    void SetSprite(sf::Sprite*);
    TypeWorldObject GetType() const;
    Unit* ToUnit();

private:
    TypeWorldObject m_Type;
    sf::Sprite* m_Tile;
    uint8 m_SizeX;
    uint8 m_SizeY;
};

