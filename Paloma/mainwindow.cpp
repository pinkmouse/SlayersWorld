#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"
#include <QFileDialog>
#include <stdio.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TilesViewScale(TILESVIEWSCALE),
    m_MapViewScale(MAPVIEWSCALE),
    m_SelectedTile(nullptr),
    m_Config(new Config())
{
    ui->setupUi(this);
    CleanButtons();
    handleLevel1();

    m_TileSet = new TileSet(m_SelectedTile);
    m_Map = new Map(m_TileSet, m_Config);
    if (!m_TileSet->SetTileSetImg("tiles.png"))
        ui->statusBar->showMessage("Error: Loading TileSet Img");


    ui->m_GraphicsViewTiles->setScene(m_TileSet);
    ui->m_GraphicsViewTiles->scale(m_TilesViewScale, m_TilesViewScale);
    if (!m_TileSet->LoadTileToScene())
        ui->statusBar->showMessage("Error: Loading Tiles to scene");
    m_TileSet->DrawGrid();

    ui->m_GraphicsViewMap->setScene(m_Map);
    ui->m_GraphicsViewMap->scale(m_MapViewScale, m_MapViewScale);
    m_Map->SetXMap(ui->m_XSizeField->value());
    m_Map->SetYMap(ui->m_YSizeField->value());
    m_Map->DrawGrid();

    // Connect button signal to appropriate slot
    connect(ui->m_TileSetButtonAdd, SIGNAL (clicked()), this, SLOT (handleButtonAddTileSet()));
    connect(ui->m_TileSetButtonSub, SIGNAL (clicked()), this, SLOT (handleButtonSubTileSet()));
    connect(ui->m_ButtonAddMap, SIGNAL (clicked()), this, SLOT (handleButtonAddMap()));
    connect(ui->m_ButtonSubMap, SIGNAL (clicked()), this, SLOT (handleButtonSubMap()));
    connect(ui->m_ButtonAddOpMap, SIGNAL (clicked()), this, SLOT (handleButtonAddOpMap()));
    connect(ui->m_ButtonSubOpMap, SIGNAL (clicked()), this, SLOT (handleButtonSubOpMap()));
    connect(ui->m_Level1, SIGNAL (clicked()), this, SLOT (handleLevel1()));
    connect(ui->m_Level2, SIGNAL (clicked()), this, SLOT (handleLevel2()));
    connect(ui->m_Level3, SIGNAL (clicked()), this, SLOT (handleLevel3()));
    connect(ui->m_Level4, SIGNAL (clicked()), this, SLOT (handleLevel4()));
    connect(ui->m_Block, SIGNAL (clicked()), this, SLOT (handleBlock()));
    connect(ui->m_YSizeField,SIGNAL(valueChanged(int)),this,SLOT(setYMap(int)));
    connect(ui->m_XSizeField,SIGNAL(valueChanged(int)),this,SLOT(setXMap(int)));
    connect(ui->m_YSizeField,SIGNAL(valueChanged(int)),this,SLOT(setYMap(int)));
    connect(ui->actionExporterMap, SIGNAL(triggered()), this, SLOT(exportMap()));
    connect(ui->actionOuvrirMap, SIGNAL(triggered()), this, SLOT(openMap()));

    ui->statusBar->showMessage("Loading Success");
}

void MainWindow::setXMap(int p_X)
{
    m_Map->SetXMap(p_X);
    m_Map->DrawGrid();
}

void MainWindow::setYMap(int p_Y)
{
    m_Map->SetYMap(p_Y);
    m_Map->DrawGrid();
}


void MainWindow::handleButtonAddTileSet()
{
    if (m_TilesViewScale + 0.2f >= 4.0f)
        return;

    m_TilesViewScale = m_TilesViewScale + 0.2f;
    ui->m_GraphicsViewTiles->setTransform(QTransform::fromScale(m_TilesViewScale, m_TilesViewScale));
    ui->statusBar->showMessage("Resize TileSet *" + QString::number(m_TilesViewScale));
}

void MainWindow::handleButtonSubTileSet()
{
    if (m_TilesViewScale - 0.2f <= 0.4f)
        return;

    m_TilesViewScale = m_TilesViewScale - 0.2f;
    ui->m_GraphicsViewTiles->setTransform(QTransform::fromScale(m_TilesViewScale, m_TilesViewScale));
    ui->statusBar->showMessage("Resize TileSet *" + QString::number(m_TilesViewScale));
}

void MainWindow::handleButtonAddMap()
{
    if (m_MapViewScale + 0.2f >= 4.0f)
        return;

    m_MapViewScale = m_MapViewScale + 0.2f;
    ui->m_GraphicsViewMap->setTransform(QTransform::fromScale(m_MapViewScale, m_MapViewScale));
    ui->statusBar->showMessage("Resize Map *" + QString::number(m_MapViewScale));
}

void MainWindow::handleButtonSubMap()
{
    if (m_MapViewScale - 0.2f <= 0.2f)
        return;

    m_MapViewScale = m_MapViewScale - 0.2f;
    ui->m_GraphicsViewMap->setTransform(QTransform::fromScale(m_MapViewScale, m_MapViewScale));
    ui->statusBar->showMessage("Resize Map *" + QString::number(m_MapViewScale));
}

void MainWindow::handleButtonAddOpMap()
{
    float l_Opacity = m_Map->GetOpacityGrid();

    if (l_Opacity + 0.1f > 1.0f)
        return;

    m_Map->SetOpacityGrid(l_Opacity + 0.1f);
}

void MainWindow::handleButtonSubOpMap()
{
    float l_Opacity = m_Map->GetOpacityGrid();

    if (l_Opacity - 0.1f < 0.0f)
        return;

    m_Map->SetOpacityGrid(l_Opacity - 0.1f);
}


void MainWindow::CleanButtons()
{
    ui->m_Level1->setStyleSheet("background-color: #FFF");
    ui->m_Level2->setStyleSheet("background-color: #FFF");
    ui->m_Level3->setStyleSheet("background-color: #FFF");
    ui->m_Level4->setStyleSheet("background-color: #FFF");
    ui->m_Block->setStyleSheet("background-color: #FFF");
}

void MainWindow::handleLevel1()
{
    m_Config->SetTileLevel(1);
    CleanButtons();
    ui->m_Level1->setStyleSheet("background-color: #00ff00");
}


void MainWindow::handleLevel2()
{
    m_Config->SetTileLevel(2);
    CleanButtons();
    ui->m_Level2->setStyleSheet("background-color: #00ff00;");
}

void MainWindow::handleLevel3()
{
    m_Config->SetTileLevel(3);
    CleanButtons();
    ui->m_Level3->setStyleSheet("background-color: #00ff00;");
}

void MainWindow::handleLevel4()
{
    m_Config->SetTileLevel(4);
    CleanButtons();
    ui->m_Level4->setStyleSheet("background-color: #00ff00;");
}

void MainWindow::handleBlock()
{
    m_Config->SetTileLevel(0);
    CleanButtons();
    ui->m_Block->setStyleSheet("background-color: #00ff00;");
}

void MainWindow::openMap()
{
    const char* l_FileName = QFileDialog::getOpenFileName(this, tr("Open Map"), "/", tr("Map (*.map)")).toStdString().c_str();

    FILE * pFile;
    pFile = fopen(l_FileName , "r");
    if (pFile == nullptr)
    {
        ui->statusBar->showMessage("Error: Open Map");
        return;
    }
    t_Param l_Param;

    /// Read Params
    fread(&l_Param, sizeof(l_Param), 1, pFile);
    int l_X = l_Param.l_Size[0];
    int l_Y = l_Param.l_Size[1];

    /// Clear and Resize
    m_Map->ClearMap();
    ui->m_XSizeField->setValue(l_X);
    ui->m_YSizeField->setValue(l_Y);
    m_Map->SetXMap(l_X);
    m_Map->SetYMap(l_Y);

    /// Draw Map
    for (int i = 0; i < (l_X * l_Y); ++i)
    {
        t_Case l_FluxCase;
        fread(&l_FluxCase, sizeof(l_FluxCase), 1, pFile);
        bool l_Block = l_FluxCase.l_Block;
        Case* l_Case = new Case(i, i % l_X, i / l_X);
        for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
        {
            if (l_FluxCase.l_TabTileNb[j] == -1)
                continue;

            Tile* l_Tile = m_TileSet->GetTile(l_FluxCase.l_TabTileNb[j]);
            Tile* l_NewTile = new Tile(l_Tile->pixmap());
            l_Case->AddTile(l_NewTile, j);
        }
        if (l_Block)
        {
            std::cout << "----> " << i << std::endl;
            l_Case->SetBlock(true);
            QGraphicsTextItem *l_Txt = new QGraphicsTextItem("B");
            l_Case->AddTxt(l_Txt);
        }
        m_Map->SetCase(l_Case);
    }
}

void MainWindow::exportMap()
{
     const char* l_FileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "/map.map", tr("Map (*.map)")).toStdString().c_str();

     FILE * pFile;
     pFile = fopen(l_FileName, "wb");

     /// Write Params
     t_Param l_Param;
     l_Param.l_Size[0] = m_Map->GetXMap();
     l_Param.l_Size[1] = m_Map->GetYMap();
     std::cout << l_Param.l_Size[0] << " " << l_Param.l_Size[1] << std::endl;
     fwrite(&l_Param, sizeof(l_Param), 1, pFile);

     /// Write Cases
     for (int i = 0; i < m_Map->GetMapSize(); ++i)
     {
         Case* l_Case = m_Map->GetCase(i);

         t_Case l_FluxCase;
         for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
         {
            Tile* l_Tile = l_Case->GetTile(j);
            if (l_Tile == nullptr)
                l_FluxCase.l_TabTileNb[j] = -1;
            else
                l_FluxCase.l_TabTileNb[j] = l_Tile->GetID();
         }
        l_FluxCase.l_Block = l_Case->GetBlock();
        fwrite(&l_FluxCase, sizeof(l_FluxCase), 1, pFile);
     }

     fclose (pFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}
