#include "map.h"
#include "define.h"
#include <iostream>
#include <QPainter>

Map::Map(TileSet *p_TileSet) :
    m_TileSet(p_TileSet),
    m_X(0),
    m_Y(0)
{
    setBackgroundBrush(Qt::black);
}

void Map::ResizeMap(int p_X, int p_Y)
{
    m_X = p_X;
    m_Y = p_Y;

    int nbCase = m_X * m_Y;
    for (int i = 0; i < nbCase; ++i)
        m_CaseList.push_back(new Case(i));
    QRectF l_Rect(0, 0, m_X * TILESIZE, m_Y * TILESIZE);
    setSceneRect(l_Rect);
}

void Map::SetXMap(int p_X)
{
    ResizeMap(p_X, m_Y);
}

void Map::SetYMap(int p_Y)
{
    ResizeMap(m_X, p_Y);
}

void Map::ClickedOnMap(const QPointF & p_Point)
{
    if (p_Point.x() < 0 || p_Point.y() < 0)
        return;

    if (p_Point.x() > m_X * TILESIZE || p_Point.y() > m_Y * TILESIZE)
        return;

    if (m_TileSet != nullptr)
    {
        Tile* l_Tile = m_TileSet->GetTile(m_TileSet->GetTileNB());
        std::cout << "Plop " << l_Tile->GetID() << " " << p_Point.x() << " " << p_Point.y() << std::endl;
    }
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *p_MouseEvent)
{
    ClickedOnMap(p_MouseEvent->scenePos());
}
