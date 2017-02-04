#include "Visual.hpp"

Visual::Visual(eVisualType p_Type, uint8 p_NbSpriteX, uint8 p_NbSpriteY) :
    m_Type(p_Type),
    m_NbSpriteX(p_NbSpriteX),
    m_NbSpriteY(p_NbSpriteY)
{
}

Visual::Visual() :
    m_NbSpriteX(0),
    m_NbSpriteY(0)
{
}

Visual::~Visual()
{
}

uint8 Visual::GetNbSpriteX() const
{
    return m_NbSpriteX;
}

uint8 Visual::GetNbSpriteY() const
{
    return m_NbSpriteY;
}

eVisualType Visual::GetType() const
{
    return m_Type;
}

void Visual::AddSprite(SkinSprite p_Sprite)
{
    m_SpriteList.push_back(p_Sprite);
}

SkinSprite* Visual::GetSprite(uint8 p_SpriteID)
{
    while (p_SpriteID >= m_SpriteList.size())
        p_SpriteID -= (MAX_MOVEMENT_POSITION * Orientation::MAX);

    p_SpriteID = std::min((uint8)m_SpriteList.size(), p_SpriteID);
    return &m_SpriteList[p_SpriteID];
}