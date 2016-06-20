#ifndef TILESET_H
#define TILESET_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QPixmap>
#include <QSize>

class TileSet : public QGraphicsScene
{
    Q_OBJECT

public:
    TileSet(QObject * p_Parent = 0);
    bool SetTileSetImg(const char*);
    bool LoadTileToScene();

    QSize GetTileSetImgSize();
    QSize GetSceneSize();

    int GetTotalTiles() const;
    void SetClickedTile(const QPointF &);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

private:
    QPixmap                             *m_TileSetImg;
    QSize                               m_SizeTileSetImg;
    int                                 m_SelectedTile;
    std::vector<QGraphicsPixmapItem*>   m_TileList;
};

#endif // TILESET_H
