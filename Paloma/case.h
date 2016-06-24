#ifndef CASE_H
#define CASE_H

#include <array>
#include "tile.h"
#include "define.h"

class Case
{
public:
    Case(int, int ,int);
    void    AddTile(Tile*, int);
    int     GetX() const;
    int     GetY() const;
    Tile*   GetTile(int) const;
    int     GetMaxTileLevel() const;

private:
    std::array<Tile*, TILENUMBERONCASE> m_TileList;
    bool                                m_Block;
    int                                 m_ID;
    int                                 m_X;
    int                                 m_Y;
};

#endif // CASE_H
