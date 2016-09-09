#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "SkinSprite.hpp"
#include <vector>

class Skin
{
public:
    Skin(uint8, uint8);
    ~Skin();
    void AddSprite(SkinSprite*);
    SkinSprite* GetSprite(uint8) const;
    uint8 GetNbSpriteX() const;
    uint8 GetNbSpriteY() const;

private:
    std::vector<SkinSprite*> m_SpriteList;
    uint8 m_NbSpriteX;
    uint8 m_NbSpriteY;
};

