#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "tileset.h"
#include "map.h"
#include "tile.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handleTileSetButtonAdd();
    void handleTileSetButtonSub();
    void setXMap(int);
    void setYMap(int);

private:
    Ui::MainWindow  *ui;
    QGraphicsScene  *m_Scene;
    TileSet         *m_TileSet;
    Map             *m_Map;
    Tile            *m_SelectedTile;
    float           m_TilesViewScale;
    float           m_MapViewScale;
};

#endif // MAINWINDOW_H
