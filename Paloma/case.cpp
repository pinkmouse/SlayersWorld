#include <iostream>
#include "case.h"

Case::Case(int p_ID, int p_X, int p_Y) :
    m_ID(p_ID),
    m_X(p_X),
    m_Y(p_Y),
    m_Block(false)
{
    for (int i = 0; i < m_TileList.size(); ++i)
        m_TileList[i] = nullptr;
    m_Txt = nullptr;
}

void    Case::AddTile(Tile* l_Tile, int l_level)
{
    m_TileList[l_level] = l_Tile;
}

void   Case::AddTxt(QGraphicsTextItem* l_Txt)
{
    m_Txt = l_Txt;
    m_Txt->setDefaultTextColor(QColor(255, 255, 255));
}

void    Case::RemoveTile(int l_Level)
{
    m_TileList[l_Level] = nullptr;
}

QGraphicsTextItem* Case::GetTxt() const
{
    return m_Txt;
}

Tile*  Case::GetTile(int l_Level) const
{
    if (l_Level <= m_TileList.size())
        return m_TileList[l_Level];
    return nullptr;
}

int     Case::GetPos() const
{
    return m_ID;
}

int     Case::GetMaxTileLevel() const
{
    return m_TileList.size();
}

void    Case::SetBlock(bool p_Block)
{
    m_Block = p_Block;
}

bool    Case::GetBlock() const
{
    return m_Block;
}

int    Case::GetX() const
{
    return m_X;
}

int    Case::GetY() const
{
    return m_Y;
}
