#pragma once

#include <cstdio>

#define IP_SERVER "82.247.125.1"
#define PORT_SERVER 1234

/// GRAPHICS
#define X_WINDOW 640
#define Y_WINDOW 480
#define ZOOM_FACTOR 0.75f

#define PLAYER_RAY 10

/// TILES
#define MAX_TILE_LEVEL 4
#define TILE_SIZE 32
#define TILE_FOLDER "img/tiles/"
#define SIZE_DRAWING_SQUARE 14

/// SKIN
#define SKIN_ZOOM_FACTOR_DEFAULT 1.33f
#define UPDATE_OPACITY_TIME 250
#define SKINS_FOLDER "img/skins/"
#define SKIN_SIZE_X 24
#define SKIN_SIZE_Y 32
#define MAX_MOVEMENT_POSITION 3
#define UPDATE_TIME_MOVEMENT 250

/// UNIT
#define TALK_TIME_SHOW 5000

/// CHATBAR
#define MAX_LETTER_STR 30

/// FONT
#define FONT_FOLDER "font/"
#define SIZE_NAME_FONT 16
#define SIZE_TALK_FONT 16

/// INTERFACE
#define IMG_FOLDER "img/window/"
#define FONT_FIELD_SIZE 128
#define SIZE_FILED_TALK_Y 32
#define FLASK_SIZE_X 16
#define FLASK_SIZE_Y 72
#define FLASK_OFFSET_TOP 22
#define FLASK_OFFSET_BOTTOM 2
#define FLASK_SCALE 2.0f


/// EVENT
#define MAX_KEY_SAVE 4

/// MAP
#define MAP_FOLDER "map/"
#define NAME_WINDOW "Slayers World"

/// GENERAL
#define STEP_SIZE 10 /// In pixel

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned long long uint64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

enum Orientation
{
    Up      = 0,
    Right   = 1,
    Down    = 2,
    Left    = 3
};

enum Action
{
    Neutral = 0,
    Attack  = 1
};

enum TypeUnit
{
    CREATURE = 0,
    PLAYER = 1
};