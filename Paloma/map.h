#ifndef MAP_H
#define MAP_H

#include <vector>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QString>
#include <QPixmap>
#include <QSize>
#include "tileset.h"
#include "case.h"
#include "config.h"

class Map : public QGraphicsScene
{
    Q_OBJECT

public:
    Map(TileSet*, Config*);
    void SetXMap(int);
    void SetYMap(int);
    int GetXMap() const;
    int GetYMap() const;

    void ClickedOnMap(const QPointF &);
    void ResizeMap(int, int);

    void DrawGrid();
    void ClearGrid();
    void SetOpacityGrid(float);
    float GetOpacityGrid() const;
    void ShowCase(Case *);
    void ClearMap();
    void ShowMap();
    int GetMapSize() const;
    Case *GetCase(int);
    void SetCase(Case*);
    Case *GetCase(std::vector<Case*>, int, int);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

private:
    TileSet*            m_TileSet;
    std::vector<Case*>  m_CaseList;
    Config*             m_Config;
    float m_GridOpacity;
    int m_X;
    int m_Y;

    /// GRID
    std::vector<QGraphicsLineItem*> m_ListLine;
};

#endif // MAP_H
