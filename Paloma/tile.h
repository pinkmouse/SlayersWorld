#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>

class Tile : public QGraphicsPixmapItem
{
public:
    Tile(const QPixmap &);
    void SetID(int);
    int GetID() const;

private:
    int m_ID;
};

#endif // TILE_H
