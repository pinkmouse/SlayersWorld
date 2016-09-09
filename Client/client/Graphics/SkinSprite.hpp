#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "../Define.hpp"

class SkinSprite : public sf::Sprite
{
public:
    SkinSprite(uint8, uint8);
    ~SkinSprite();

private:
    uint8 m_NbSkinX;
    uint8 m_NbSkinY;
};

