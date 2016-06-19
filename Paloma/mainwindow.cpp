#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

#define TILESVIEWSCALE 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TilesViewScale(TILESVIEWSCALE)
{
    ui->setupUi(this);
    m_Scene = new QGraphicsScene(this);
    m_TileSet = new TileSet(m_Scene);
    if (m_TileSet->SetTileSetImg("tiles.png"))
        ui->statusBar->messageChanged("Error: Loading TileSet Img");
    else
        ui->statusBar->messageChanged("Loading TileSet Img done");
    ui->m_GraphicsViewTiles->setScene(m_Scene);
    ui->m_GraphicsViewTiles->scale(m_TilesViewScale, m_TilesViewScale);

    QSize l_SceneSize = m_TileSet->GetSceneSize();
    m_Scene->setSceneRect(0, 0, l_SceneSize.width(), l_SceneSize.height());
    m_TileSet->LoadTileToScene();

    // Connect button signal to appropriate slot
    connect(ui->m_TileSetButtonAdd, SIGNAL (clicked()), this, SLOT (handleTileSetButtonAdd()));
    connect(ui->m_TileSetButtonSub, SIGNAL (clicked()), this, SLOT (handleTileSetButtonSub()));
}


void MainWindow::handleTileSetButtonAdd()
{
    if (m_TilesViewScale + 0.2f >= 2.0f)
        return;

    m_TilesViewScale = m_TilesViewScale + 0.2f;
    ui->m_GraphicsViewTiles->setTransform(QTransform::fromScale(m_TilesViewScale, m_TilesViewScale));

    std::cout << "Add : " << m_TilesViewScale << std::endl;
}

void MainWindow::handleTileSetButtonSub()
{
    if (m_TilesViewScale - 0.2f <= 0.4f)
        return;

    m_TilesViewScale = m_TilesViewScale - 0.2f;
    ui->m_GraphicsViewTiles->setTransform(QTransform::fromScale(m_TilesViewScale, m_TilesViewScale));

    std::cout << "Sub : " << m_TilesViewScale << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}
