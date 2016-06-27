#ifndef CASE_H
#define CASE_H

#include <array>
#include <QGraphicsTextItem>
#include "tile.h"
#include "define.h"

class Case
{
public:
    Case(int, int ,int);
    void    AddTile(Tile*, int);
    void    RemoveTile(int);
    void    AddTxt(QGraphicsTextItem*);
    int     GetX() const;
    int     GetY() const;
    int     GetPos() const;
    Tile*   GetTile(int) const;
    int     GetMaxTileLevel() const;
    void    SetBlock(bool);
    bool    GetBlock() const;
    QGraphicsTextItem* GetTxt() const;

private:
    std::array<Tile*, TILENUMBERONCASE> m_TileList;
    QGraphicsTextItem*                  m_Txt;
    bool                                m_Block;
    int                                 m_ID;
    int                                 m_X;
    int                                 m_Y;
};

#endif // CASE_H
