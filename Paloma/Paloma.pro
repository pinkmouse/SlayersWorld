#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T21:45:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paloma
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tileset.cpp \
    map.cpp \
    tile.cpp \
    case.cpp \
    config.cpp

HEADERS  += mainwindow.h \
    tileset.h \
    map.h \
    define.h \
    tile.h \
    case.h \
    config.h

FORMS    += mainwindow.ui
