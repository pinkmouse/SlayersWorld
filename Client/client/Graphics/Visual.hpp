#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "SkinSprite.hpp"
#include <vector>

class Visual
{
public:
    Visual();
    Visual(eVisualType, uint8, uint8);
    ~Visual();
    void AddSprite(SkinSprite);
    SkinSprite* GetSprite(uint8);
    uint8 GetNbSpriteX() const;
    uint8 GetNbSpriteY() const;
    eVisualType GetType() const;

private:
    eVisualType m_Type;
    std::vector<SkinSprite> m_SpriteList;
    uint8 m_NbSpriteX;
    uint8 m_NbSpriteY;
};

