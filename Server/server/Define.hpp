#pragma once
#include <string>
#include <vector>

#define MAP_PATH "map/"
#define CONF_FILE "server.conf"

#define GRID_SIZE 12
#define TILE_SIZE 16

#define IN_MILLISECOND 1000
#define IN_MICROSECOND 1000 * IN_MILLISECOND

#define PLAYER_TIME_RESPAWN 9
#define MAX_HEALTH 100
#define REGEN_HEALTH_TIMER 5000

#define UPDATE_TIME_MOVEMENT 200
#define STEP_SIZE 8 /// In pixel
#define MAX_MOVEMENT_POSITION 3

#define OUT_OF_COMBAT_TIME 5
#define MELEE_RANGE TILE_SIZE

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

enum TypeUnit
{
    CREATURE = 0,
    PLAYER = 1
};

enum eAiType
{
    PASSIVE = 0,
    DEFENSIVE = 1,
    AGRESIVE = 2
};

enum eActionType
{
    Go = 0,
    Attack = 1,
    Stop = 2,
    StopAttack = 3
};

enum eAccessType
{
    Dummy = 0,
    Moderator = 1,
    Administrator = 2
};

enum eFactionType
{
    Ally = 0,
    Enemy = 1,
    Neutral = 2
};

enum eGossipType
{
    Whisp = 0,
    Yell = 1
};

struct Position 
{
    uint32 m_X;
    uint32 m_Y;

    Position() :
    m_X(0), m_Y(0) {}

    Position(uint32 p_X, uint32 p_Y) :
    m_X(p_X), m_Y(p_Y) {}

    Position(const Position & p_Position) :
    m_X(p_Position.m_X), m_Y(p_Position.m_Y) {}
};

struct Gossip
{
    uint16 m_ID;
    TypeUnit m_TypeUnit;
    uint16 m_UnitEntry;
    eGossipType m_GossipType;
    uint32 m_Data1;
    std::string m_Msg;
    uint64 m_GossipTimer;

    Gossip() :
        m_ID(0), m_TypeUnit(TypeUnit::CREATURE), m_UnitEntry(0), m_GossipType(eGossipType::Whisp), m_Data1(0), m_Msg(""), m_GossipTimer(0) {}

    Gossip(const Gossip & p_Gossip) :
        m_ID(p_Gossip.m_ID), m_TypeUnit(p_Gossip.m_TypeUnit), m_UnitEntry(p_Gossip.m_UnitEntry), m_GossipType(p_Gossip.m_GossipType), m_Data1(p_Gossip.m_Data1), m_Msg(p_Gossip.m_Msg), m_GossipTimer(p_Gossip.m_GossipTimer){}
        
    Gossip(uint16 p_ID, TypeUnit p_TypeUnit, uint16 p_UnitEntry, eGossipType p_GossipType, uint32 p_Data1, std::string p_Msg) :
        m_ID(p_ID), m_TypeUnit(p_TypeUnit), m_UnitEntry(p_UnitEntry), m_GossipType(p_GossipType), m_Data1(p_Data1), m_Msg(p_Msg), m_GossipTimer(0){}
};

struct CreatureTemplate
{
    uint32 m_Entry;
    uint8 m_SkinID;
    std::string m_Name;
    uint8 m_Level;
    uint8 m_Force;
    uint8 m_Stamina;
    uint8 m_Dexterity;
    uint8 m_Xp;
    uint8 m_State;
    uint16 m_MaxRay;
    uint16 m_RespawnTime;
    uint8 m_Rank;
    uint8 m_AiType;
    eFactionType m_FactionType;

    CreatureTemplate() :
    m_Entry(0), m_SkinID(0), m_Name(""), m_Level(0), m_Force(0), m_Stamina(0), m_Dexterity(0), m_Xp(0), m_State(0), m_MaxRay(0), m_RespawnTime(0), m_Rank(0), m_AiType(0), m_FactionType(eFactionType::Ally){}

    CreatureTemplate(uint32 p_Entry, uint8 p_SkinID, std::string p_Name, uint8 p_Level, uint8 p_Force, uint8 p_Stamina, uint8 p_Dexterity, uint8 p_Xp, uint8 p_State, uint16 p_MaxRay, uint16 p_RespawnTime, uint8 p_Rank, uint8 p_AiType, eFactionType p_FactionType) :
    m_Entry(p_Entry), m_SkinID(p_SkinID), m_Name(p_Name), m_Level(p_Level), m_Force(p_Force), m_Stamina(p_Stamina), m_Dexterity(p_Dexterity), m_Xp(p_Xp), m_State(p_State), m_MaxRay(p_MaxRay), m_RespawnTime(p_RespawnTime), m_Rank(p_Rank), m_AiType(p_AiType), m_FactionType(p_FactionType){}
};

struct PointsSet
{
	uint16 m_FreePoints;
	uint16 m_Force;
	uint16 m_Stamina;
	uint16 m_Dexterity;

	PointsSet() :
		m_FreePoints(0), m_Force(0), m_Stamina(0), m_Dexterity(0){}

	PointsSet(uint16 p_FreePoints, uint16 p_Force, uint16 p_Stamina, uint16 p_Dexterity) :
		m_FreePoints(p_FreePoints), m_Force(p_Force), m_Stamina(p_Stamina), m_Dexterity(p_Dexterity) {}

	PointsSet(uint16 p_Force, uint16 p_Stamina, uint16 p_Dexterity) :
		m_FreePoints(0), m_Force(p_Force), m_Stamina(p_Stamina), m_Dexterity(p_Dexterity) {}

	void SetFreePoints(uint16 p_FreePoints) { m_FreePoints = p_FreePoints; };
	void SetForce(uint16 p_Force) { m_Force = p_Force; };
	void SetStamina(uint16 p_Stamina) { m_Stamina = p_Stamina; };
	void SetDexterity(uint16 p_Dexterity) { m_Dexterity = p_Dexterity; };
};


static uint32 PixelToCase(uint32 p_NbPixel) { return p_NbPixel / TILE_SIZE; }
static uint32 CaseToPixel(uint32 p_NbCase) { return p_NbCase * TILE_SIZE; }