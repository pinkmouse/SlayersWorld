#include "tile.h"

Tile::Tile(const QPixmap &p_Pixmap) :
    QGraphicsPixmapItem(p_Pixmap),
    m_ID(0)
{

}

void Tile::SetID(int p_ID)
{
    m_ID = p_ID;
}

int Tile::GetID() const
{
    return m_ID;
}
