#include "map.h"
#include "define.h"
#include <iostream>
#include <QPainter>

Map::Map(TileSet *p_TileSet, Config *p_Config) :
    m_TileSet(p_TileSet),
    m_Config(p_Config),
    m_X(0),
    m_Y(0)
{
    setBackgroundBrush(Qt::black);
}

void Map::DrawGrid()
{
    QPen l_Pen(QColor(255, 255, 255, 200));
    l_Pen.setWidth(1);

    for (int i = 0; i < m_ListLine.size(); ++i)
        removeItem(m_ListLine[i]);

    m_ListLine.clear();

    /// Vertical
    for (int i = 0; i <= m_X; ++i)
    {
        QGraphicsLineItem* l_Line = addLine(i * TILESIZE, 0, i * TILESIZE, m_Y * TILESIZE, l_Pen);
        l_Line->setZValue(2);
        m_ListLine.push_back(l_Line);
    }

    /// Horizontal
    for (int i = 0; i <= m_Y; ++i)
    {
        QGraphicsLineItem* l_Line = addLine(0, i * TILESIZE, m_X *TILESIZE , i * TILESIZE, l_Pen);
        l_Line->setZValue(2);
        m_ListLine.push_back(l_Line);
    }
}

void Map::ResizeMap(int p_X, int p_Y)
{
    m_X = p_X;
    m_Y = p_Y;

    m_CaseList.clear();
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
    if (m_TileSet == nullptr)
        return;

    if (p_Point.x() < 0 || p_Point.y() < 0)
        return;

    if (p_Point.x() > m_X * TILESIZE || p_Point.y() > m_Y * TILESIZE)
        return;

    int l_XClicked = p_Point.x() / TILESIZE;
    int l_YClicked = p_Point.y() / TILESIZE;

    int l_NbSelectedTile = (m_X * l_YClicked) + l_XClicked;

    Case* l_Case = m_CaseList[l_NbSelectedTile];
    Tile* l_Tile = m_TileSet->GetTile(m_TileSet->GetTileNB());

    Tile* l_NewTile = new Tile(l_Tile->pixmap());
    l_NewTile->SetID(l_Tile->GetID());
    l_Case->AddTile(l_NewTile, m_Config->GetTileLevel());
    addItem(l_NewTile);
    l_NewTile->setZValue(m_Config->GetTileLevel());
    l_NewTile->setPos(l_XClicked * TILESIZE, l_YClicked * TILESIZE);

    std::cout << "Plop " << l_Tile->GetID() << " " << p_Point.x() << " " << p_Point.y() << " " << m_Config->GetTileLevel() << std::endl;
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *p_MouseEvent)
{
    ClickedOnMap(p_MouseEvent->scenePos());
}
