#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_Scene = new QGraphicsScene(this);
    ui->m_GraphicsViewTiles->setScene(m_Scene);

    QPixmap* l_PixmapTiles = new QPixmap("tiles.png");
    QRect l_Rect(32, 32, 32, 32);
    QGraphicsPixmapItem* l_Tiles = m_Scene->addPixmap(l_PixmapTiles->copy(l_Rect)); // Ajout à la scène
    l_Tiles->setPos(32, 32); // Positionne le joueur à la bonne place
    l_Tiles->setZValue(5); // Affiche au dessus du décor
}

MainWindow::~MainWindow()
{
    delete ui;
}
