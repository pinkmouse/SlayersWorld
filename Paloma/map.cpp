#include "map.h"
#include "define.h"
#include <iostream>
#include <QPainter>

Map::Map(TileSet *p_TileSet, Config *p_Config) :
    m_TileSet(p_TileSet),
    m_Config(p_Config),
    m_X(0),
    m_Y(0),
    m_GridOpacity(0.8)
{
    setBackgroundBrush(Qt::black);
}

void Map::ClearGrid()
{
    for (int i = 0; i < m_ListLine.size(); ++i)
        removeItem(m_ListLine[i]);

    m_ListLine.clear();
}

void Map::DrawGrid()
{
    QPen l_Pen(QColor(255, 255, 255, 200));
    l_Pen.setWidth(1);

    ClearGrid();

    /// Vertical
    for (int i = 0; i <= m_X; ++i)
    {
        QGraphicsLineItem* l_Line = addLine(i * TILESIZE, 0, i * TILESIZE, m_Y * TILESIZE, l_Pen);
        l_Line->setZValue(100);
        l_Line->setOpacity(m_GridOpacity);
        m_ListLine.push_back(l_Line);
    }

    /// Horizontal
    for (int i = 0; i <= m_Y; ++i)
    {
        QGraphicsLineItem* l_Line = addLine(0, i * TILESIZE, m_X *TILESIZE , i * TILESIZE, l_Pen);
        l_Line->setZValue(100);
        l_Line->setOpacity(m_GridOpacity);
        m_ListLine.push_back(l_Line);
    }
}

int Map::GetXMap() const
{
    return m_X;
}

int Map::GetYMap() const
{
    return m_Y;
}

void Map::SetOpacityGrid(float m_Opacity)
{
    m_GridOpacity = m_Opacity;
    for (int i = 0; i < m_ListLine.size(); ++i)
    {
        QGraphicsItem* l_Item = m_ListLine[i];
        l_Item->setOpacity(m_GridOpacity);
    }
}

float Map::GetOpacityGrid() const
{
    return m_GridOpacity;
}

Case* Map::GetCase(std::vector<Case *> l_CaseList, int p_X, int p_Y)
{
    for (int i = 0; i < l_CaseList.size(); ++i)
    {
        if (l_CaseList[i]->GetX() == p_X && l_CaseList[i]->GetY() == p_Y)
            return l_CaseList[i];
    }
    return nullptr;
}

Case* Map::GetCase(int p_Nb)
{
    if (p_Nb > m_CaseList.size())
        return nullptr;

    return m_CaseList[p_Nb];
}

int Map::GetMapSize() const
{
    return m_CaseList.size();
}

void Map::ShowCase(Case *l_Case)
{
    for (int i = 0; i < l_Case->GetMaxTileLevel(); ++i)
    {
        Tile* l_Tile = l_Case->GetTile(i);
        if (l_Tile != nullptr)
        {
            addItem(l_Tile);
            l_Tile->setZValue(i);
            l_Tile->setPos(l_Case->GetX() * TILESIZE, l_Case->GetY() * TILESIZE);
        }
    }
    QGraphicsTextItem* l_Txt = l_Case->GetTxt();
    if (l_Txt != nullptr)
    {
        addItem(l_Txt);
        l_Txt->setZValue(l_Case->GetMaxTileLevel() + 1);
        l_Txt->setPos(l_Case->GetX() * TILESIZE, l_Case->GetY() * TILESIZE);
    }
}

void Map::ShowMap()
{
    for (int i = 0; i < m_CaseList.size(); ++i)
    {
        Case* l_Case = m_CaseList[i];
        if (l_Case != nullptr)
        {
            for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
            {
                Tile* l_Tile = l_Case->GetTile(j);
                if (l_Tile != nullptr)
                {
                    addItem(l_Tile);
                }
            }
            QGraphicsTextItem* l_Txt = l_Case->GetTxt();
            if (l_Txt != nullptr)
                addItem(l_Txt);
        }
    }
}

void Map::ClearMap()
{
    for (int i = 0; i < m_CaseList.size(); ++i)
    {
        Case* l_Case = m_CaseList[i];
        if (l_Case != nullptr)
        {
            for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
            {
                Tile* l_Tile = l_Case->GetTile(j);
                if (l_Tile != nullptr)
                    removeItem(l_Tile);
            }
            QGraphicsTextItem* l_Txt = l_Case->GetTxt();
            if (l_Txt != nullptr)
                removeItem(l_Txt);
        }
    }
    m_CaseList.clear();
}

void Map::ResizeMap(int p_X, int p_Y)
{
    m_X = p_X;
    m_Y = p_Y;

    std::vector<Case*>  l_OldCaseList;
    if (!m_CaseList.empty())
    {
        for (int i = 0; i < m_CaseList.size(); ++i)
            l_OldCaseList.push_back(m_CaseList[i]);
    }

    ClearMap();
    int nbCase = m_X * m_Y;
    for (int i = 0; i < nbCase; ++i)
    {
        int l_X = i % m_X;
        int l_Y = i / m_X;

        if (Case* l_Case = GetCase(l_OldCaseList, l_X, l_Y))
            m_CaseList.push_back(l_Case);
        else
            m_CaseList.push_back(new Case(i, l_X, l_Y));
    }
    QRectF l_Rect(0, 0, m_X * TILESIZE, m_Y * TILESIZE);
    setSceneRect(l_Rect);
    ShowMap();
}

void Map::SetCase(Case* p_Case)
{
    int l_Pos = p_Case->GetPos();
    Case* l_Case = m_CaseList[l_Pos];

    if (l_Case == nullptr)
        return;

    for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
    {
        Tile* l_Tile = l_Case->GetTile(j);
        if (l_Tile != nullptr)
            removeItem(l_Tile);
    }
    if (l_Case->GetTxt() != nullptr)
        removeItem(l_Case->GetTxt());

    l_Case = p_Case;
    for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
    {
       Tile* l_Tile = l_Case->GetTile(j);
       if (l_Tile == nullptr)
           continue;

       addItem(l_Tile);
       l_Tile->setZValue(j);
       l_Tile->setPos(l_Case->GetX() * TILESIZE, l_Case->GetY() * TILESIZE);
    }
    QGraphicsTextItem* l_Txt = l_Case->GetTxt();
    if (l_Txt != nullptr)
    {
        addItem(l_Txt);
        l_Txt->setZValue(l_Case->GetMaxTileLevel() + 1);
        l_Txt->setPos(l_Case->GetX() * TILESIZE, l_Case->GetY() * TILESIZE);
    }
    m_CaseList[l_Pos] = l_Case;
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

    if (m_Config->GetTileLevel())
    {
        Tile* l_Tile = m_TileSet->GetTile(m_TileSet->GetTileNB());

        Tile* l_NewTile = new Tile(l_Tile->pixmap());
        l_NewTile->SetID(l_Tile->GetID());
        l_Case->AddTile(l_NewTile, m_Config->GetTileLevel());
        addItem(l_NewTile);
        l_NewTile->setZValue(m_Config->GetTileLevel());
        l_NewTile->setPos(l_XClicked * TILESIZE, l_YClicked * TILESIZE);
    }
    else
    {
        l_Case->SetBlock(!l_Case->GetBlock());
        if (l_Case->GetBlock())
        {
            QGraphicsTextItem *l_Txt = addText("B");
            l_Txt->setPos(l_XClicked * TILESIZE, l_YClicked * TILESIZE);
            l_Txt->setDefaultTextColor(QColor(255, 255, 255));
            l_Txt->setZValue(l_Case->GetMaxTileLevel() + 1);
            l_Case->AddTxt(l_Txt);
        }
        else
        {
           removeItem(l_Case->GetTxt());
        }
    }
}

void Map::mousePressEvent(QGraphicsSceneMouseEvent *p_MouseEvent)
{
    ClickedOnMap(p_MouseEvent->scenePos());
}
