#pragma once

#include <cstdio>

#define X_WINDOW 640
#define Y_WINDOW 480

#define PLAYER_RAY 10

#define MAX_TILE_LEVEL 4
#define TILE_SIZE 16
#define TILE_FOLDER "img/tiles/"
#define SIZE_DRAWING_SQUARE 6

#define SKINS_FOLDER "img/skins/"
#define SKIN_SIZE_X 24
#define SKIN_SIZE_Y 32

#define MAP_FOLDER "map/"
#define NAME_WINDOW "Slayers World"
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
    Top     = 0,
    Right   = 1,
    Left    = 2,
    Bottom  = 3
};

enum Action
{
    Neutral = 0,
    Attack  = 1
};