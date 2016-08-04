#pragma once

#define MAP_PATH "map/"
#define CONF_FILE "server.conf"

#define SIZE_SENDING_SQUARE 12
#define TILE_SIZE 16

#define UPDATE_TIME_MOVEMENT 250
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
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

enum Action
{
    Neutral = 0,
    Attack = 1
};

enum TypeUnit
{
    CREATURE = 0,
    PLAYER = 1
};
