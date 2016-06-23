#ifndef TILESET_H
#define TILESET_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QPixmap>
#include <QSize>
#include "tile.h"

class TileSet : public QGraphicsScene
{
    Q_OBJECT

public:
    TileSet(Tile*, QObject * p_Parent = 0);
    bool SetTileSetImg(const char*);
    bool LoadTileToScene();
    void DrawGrid();

    QSize GetTileSetImgSize();
    QSize GetSceneSize();

    int GetTotalTiles() const;
    int GetTileNB() const;
    Tile* GetTile(int) const;
    void SetClickedTile(const QPointF &);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

private:
    Tile*               m_SelectedTile;
    QPixmap*            m_TileSetImg;
    QSize               m_SizeTileSetImg;
    int                 m_X;
    int                 m_Y;
    int                 m_NbSelectedTile;
    std::vector<Tile*>  m_TileList;
};

#endif // TILESET_H
