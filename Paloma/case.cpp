#include "case.h"

Case::Case(int p_ID) :
    m_ID(p_ID),
    m_Block(false)
{
}

void    Case::AddTile(Tile* l_Tile, int l_level)
{
    m_TileList[l_level] = l_Tile;
}
