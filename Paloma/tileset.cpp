#include <iostream>
#include "tileset.h"

#define TILESIZE 16

TileSet::TileSet(QGraphicsScene *p_Scene)
{
    m_Scene = p_Scene;
}

bool TileSet::SetTileSetImg(const char* p_Str)
{
    m_TileSetImg = new QPixmap(p_Str);
    if (m_TileSetImg == nullptr)
        return false;
    m_SizeTileSetImg = m_TileSetImg->size();
    return true;
}

bool TileSet::LoadTileToScene()
{
    std::cout << "Height = " << m_SizeTileSetImg.height() << ", With = " << m_SizeTileSetImg.width() << std::endl;
    QRectF l_SceneRect = m_Scene->sceneRect();

    for (int i = 0; i < GetTotalTiles(); ++i)
    {
        int l_XCrop = i % (m_SizeTileSetImg.width() / TILESIZE);
        int l_YCrop = i / (m_SizeTileSetImg.width() / TILESIZE);
        QRect l_Rect(l_XCrop * TILESIZE, l_YCrop * TILESIZE, TILESIZE, TILESIZE);

        QGraphicsPixmapItem* l_Tile = m_Scene->addPixmap(m_TileSetImg->copy(l_Rect));

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
    /*return QSize(p_ViewSize.width() / p_ScaleRatio, (GetTotalTiles() / (p_ViewSize.width() / (TILESIZE * p_ScaleRatio))) * (TILESIZE * p_ScaleRatio));*/
}

int TileSet::GetTotalTiles() const
{
    return (m_SizeTileSetImg.height() / TILESIZE * m_SizeTileSetImg.width() / TILESIZE);
}
