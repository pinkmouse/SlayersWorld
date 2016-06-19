#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "tileset.h"

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

private:
    Ui::MainWindow  *ui;
    QGraphicsScene  *m_Scene;
    TileSet         *m_TileSet;
    float           m_TilesViewScale;
};

#endif // MAINWINDOW_H
