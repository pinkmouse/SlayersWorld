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
    m_TileSet = new TileSet(this);
    if (!m_TileSet->SetTileSetImg("tiles.png"))
        ui->statusBar->showMessage("Error: Loading TileSet Img");


    ui->m_GraphicsViewTiles->setScene(m_TileSet);
    ui->m_GraphicsViewTiles->scale(m_TilesViewScale, m_TilesViewScale);
    if (!m_TileSet->LoadTileToScene())
        ui->statusBar->showMessage("Error: Loading Tiles to scene");

    // Connect button signal to appropriate slot
    connect(ui->m_TileSetButtonAdd, SIGNAL (clicked()), this, SLOT (handleTileSetButtonAdd()));
    connect(ui->m_TileSetButtonSub, SIGNAL (clicked()), this, SLOT (handleTileSetButtonSub()));

    ui->statusBar->showMessage("Loading Success");
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
