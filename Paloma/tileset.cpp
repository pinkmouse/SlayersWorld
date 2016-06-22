#include <iostream>
#include "tileset.h"
#include "define.h"

TileSet::TileSet(Tile *p_SelectedTile, QObject *p_Parent) :
    QGraphicsScene(p_Parent),
    m_SelectedTile(p_SelectedTile),
    m_NbSelectedTile(0)
{
    m_TileList.clear();
    setBackgroundBrush(Qt::black);
}

bool TileSet::SetTileSetImg(const char* p_Str)
{
    m_TileSetImg = new QPixmap(p_Str);
    if (m_TileSetImg == nullptr)
        return false;

    m_SizeTileSetImg = m_TileSetImg->size();

    QSize l_SceneSize = GetSceneSize();
    setSceneRect(0, 0, l_SceneSize.width(), l_SceneSize.height());

    return true;
}

bool TileSet::LoadTileToScene()
{
    m_TileList.clear();
    QRectF l_SceneRect = sceneRect();

    for (int i = 0; i < GetTotalTiles(); ++i)
    {
        int l_XCrop = i % (m_SizeTileSetImg.width() / TILESIZE);
        int l_YCrop = i / (m_SizeTileSetImg.width() / TILESIZE);
        QRect l_Rect(l_XCrop * TILESIZE, l_YCrop * TILESIZE, TILESIZE, TILESIZE);

        Tile* l_Tile = (Tile*)addPixmap(m_TileSetImg->copy(l_Rect));
        l_Tile->SetID(i);
        if (l_Tile == nullptr)
            return false;

        m_TileList.push_back(l_Tile);

        int l_XPast = i % ((int)l_SceneRect.width() / TILESIZE);
        int l_YPast = i / ((int)l_SceneRect.width() / TILESIZE);
        l_Tile->setPos(l_XPast * TILESIZE, l_YPast * TILESIZE);
        l_Tile->setZValue(1);
    }
    return true;
}

QSize TileSet::GetSceneSize()
{
    return QSize(m_SizeTileSetImg.width(), m_SizeTileSetImg.height());
}

int TileSet::GetTileNB() const
{
    return m_NbSelectedTile;
}

Tile* TileSet::GetTile(int p_Nb) const
{
    return m_TileList[p_Nb];
}

int TileSet::GetTotalTiles() const
{
    return (m_SizeTileSetImg.height() / TILESIZE * m_SizeTileSetImg.width() / TILESIZE);
}

void TileSet::SetClickedTile(const QPointF & p_Point)
{
    if (p_Point.x() < 0 || p_Point.y() < 0)
    {
        m_NbSelectedTile = 0;
        return;
    }

    int l_PosXTile = p_Point.x() / TILESIZE;
    int l_PosYTile = p_Point.y() / TILESIZE;

    m_NbSelectedTile = ((m_SizeTileSetImg.width() / TILESIZE) * l_PosYTile) + l_PosXTile;
    m_SelectedTile = GetTile(m_NbSelectedTile);
    std::cout << "NEW SELECTION " << m_SelectedTile->GetID() << std::endl;
    if (m_NbSelectedTile > (GetTotalTiles() - 1) || m_NbSelectedTile < 0)
        m_NbSelectedTile = 0;
}

void TileSet::mousePressEvent(QGraphicsSceneMouseEvent *p_MouseEvent)
{
    std::cout << "NEW SELECTION " << std::endl;
    SetClickedTile(p_MouseEvent->scenePos());
}
