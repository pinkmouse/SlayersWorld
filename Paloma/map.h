#ifndef MAP_H
#define MAP_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QString>
#include <QPixmap>
#include <QSize>
#include "tileset.h"
#include "case.h"

class Map : public QGraphicsScene
{
    Q_OBJECT

public:
    Map(TileSet*);
    void SetXMap(int);
    void SetYMap(int);
    void ClickedOnMap(const QPointF &);
    void ResizeMap(int, int);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

private:
    TileSet*            m_TileSet;
    std::vector<Case*>  m_CaseList;
    int m_X;
    int m_Y;
};

#endif // MAP_H
