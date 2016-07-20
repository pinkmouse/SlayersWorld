#pragma once

#define MAP_PATH "map/"
#define CONF_FILE "server.conf"

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
    Top = 0,
    Right = 1,
    Left = 2,
    Bottom = 3
};

enum Action
{
    Neutral = 0,
    Attack = 1
};