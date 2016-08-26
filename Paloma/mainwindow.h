#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "tileset.h"
#include "map.h"
#include "tile.h"
#include "config.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CleanButtons();

private slots:
    void handleButtonAddTileSet();
    void handleButtonSubTileSet();
    void handleButtonAddMap();
    void handleButtonSubMap();
    void handleButtonAddOpMap();
    void handleButtonSubOpMap();
    void handleLevel1();
    void handleLevel2();
    void handleLevel3();
    void handleLevel4();
    void handleBlock();
    void setXMap(int);
    void setYMap(int);
    void setIDMap(int);
    void setBrushSize(int);
    void exportMap();
    void openMap();
    void newMap();

private:
    Ui::MainWindow  *ui;
    QGraphicsScene  *m_Scene;
    TileSet         *m_TileSet;
    Map             *m_Map;
    Tile            *m_SelectedTile;
    Config          *m_Config;
    float           m_TilesViewScale;
    float           m_MapViewScale;

    struct          t_Case
    {
        int l_TabTileNb[4];
        bool l_Block;
    };

    struct          t_Param
    {
        int l_ID;
        int l_Size[2];
    };
};

#endif // MAINWINDOW_H
