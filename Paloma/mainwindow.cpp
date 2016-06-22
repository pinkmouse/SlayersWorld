#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TilesViewScale(TILESVIEWSCALE),
    m_MapViewScale(MAPVIEWSCALE),
    m_SelectedTile(nullptr)
{
    ui->setupUi(this);
    m_TileSet = new TileSet(m_SelectedTile);
    m_Map = new Map(m_TileSet);
    if (!m_TileSet->SetTileSetImg("tiles.png"))
        ui->statusBar->showMessage("Error: Loading TileSet Img");


    ui->m_GraphicsViewTiles->setScene(m_TileSet);
    ui->m_GraphicsViewTiles->scale(m_TilesViewScale, m_TilesViewScale);
    if (!m_TileSet->LoadTileToScene())
        ui->statusBar->showMessage("Error: Loading Tiles to scene");

    ui->m_GraphicsViewMap->setScene(m_Map);
    ui->m_GraphicsViewMap->scale(m_MapViewScale, m_MapViewScale);
    m_Map->SetXMap(ui->m_XSizeField->value());
    m_Map->SetYMap(ui->m_YSizeField->value());

    // Connect button signal to appropriate slot
    connect(ui->m_TileSetButtonAdd, SIGNAL (clicked()), this, SLOT (handleTileSetButtonAdd()));
    connect(ui->m_TileSetButtonSub, SIGNAL (clicked()), this, SLOT (handleTileSetButtonSub()));
    connect(ui->m_XSizeField,SIGNAL(valueChanged(int)),this,SLOT(setXMap(int)));
    connect(ui->m_YSizeField,SIGNAL(valueChanged(int)),this,SLOT(setYMap(int)));

    ui->statusBar->showMessage("Loading Success");
}

void MainWindow::setXMap(int p_X)
{

}

void MainWindow::setYMap(int p_Y)
{

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
