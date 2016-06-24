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
}

void    Case::AddTile(Tile* l_Tile, int l_level)
{
    m_TileList[l_level] = l_Tile;
}

Tile*  Case::GetTile(int l_Level) const
{
    if (l_Level <= m_TileList.size())
        return m_TileList[l_Level];
    return nullptr;
}

int     Case::GetMaxTileLevel() const
{
    return m_TileList.size();
}

int    Case::GetX() const
{
    return m_X;
}

int    Case::GetY() const
{
    return m_Y;
}
