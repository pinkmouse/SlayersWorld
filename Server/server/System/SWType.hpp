#pragma once

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned long long uint64;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef std::pair<int16, int16> PFNodePosition;

union CharOn11111111 {
    struct {
        unsigned int first : 1;
        unsigned int second : 1;
        unsigned int third : 1;
        unsigned int fourth : 1;
        unsigned int fifth : 1;
        unsigned int sixth : 1;
        unsigned int seventh : 1;
        unsigned int eighth : 1;
    } charOn11111111;

    uint8 m_Byte_value;
};

union CharOn116 {
    struct {
        unsigned int first : 1;
        unsigned int second : 1;
        unsigned int third : 6;
    } charOn116;
    uint8 m_Byte_value;
};