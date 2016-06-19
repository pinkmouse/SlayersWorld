#ifndef TILESET_H
#define TILESET_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QString>
#include <QPixmap>
#include <QSize>

class TileSet
{
public:
    TileSet(QGraphicsScene *);
    bool SetTileSetImg(const char*);
    bool LoadTileToScene();

    QSize GetTileSetImgSize();
    QSize GetSceneSize();

    int GetTotalTiles() const;

private:
    QGraphicsScene *m_Scene;
    QPixmap        *m_TileSetImg;
    QSize           m_SizeTileSetImg;
};

#endif // TILESET_H
