#include "Skin.hpp"



Skin::Skin(uint8 p_NbSpriteX, uint8 p_NbSpriteY) :
    m_NbSpriteX(p_NbSpriteX),
    m_NbSpriteY(p_NbSpriteY)
{
}


Skin::~Skin()
{
}

uint8 Skin::GetNbSpriteX() const
{
    return m_NbSpriteX;
}

uint8 Skin::GetNbSpriteY() const
{
    return m_NbSpriteY;
}

void Skin::AddSprite(SkinSprite* p_Sprite)
{
    m_SpriteList.push_back(p_Sprite);
}

SkinSprite* Skin::GetSprite(uint8 p_SpriteID)
{
    while (p_SpriteID >= m_SpriteList.size())
        p_SpriteID -= (MAX_MOVEMENT_POSITION * Orientation::MAX);

    if (p_SpriteID < 0)
        p_SpriteID = 0;

    return m_SpriteList[p_SpriteID];
}