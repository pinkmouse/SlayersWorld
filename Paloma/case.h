#ifndef CASE_H
#define CASE_H

#include <array>
#include "tile.h"
#include "define.h"

class Case
{
public:
    Case(int);

private:
    std::array<Tile*, TILENUMBERONCASE> m_TileList;
    bool                                m_Block;
    int                                 m_ID;
};

#endif // CASE_H
