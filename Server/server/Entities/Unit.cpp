#include "Unit.hpp"


Unit::Unit()
{
    m_Name = "";
    m_MapID = 0;
    m_PosX = 0;
    m_PosY = 0;
}


Unit::~Unit()
{
}

std::string Unit::GetName() const
{
    return m_Name;
}

uint16 Unit::GetMapID() const
{
    return m_MapID;
}

uint32 Unit::GetPosX() const
{
    return m_PosX;
}

uint32 Unit::GetPosY() const
{
    return m_PosY;
}

uint8 Unit::GetLevel() const
{
    return m_Level;
}

uint8 Unit::GetSkinID() const
{
    return m_SkinID;
}

uint8 Unit::GetOrientation() const
{
    return m_Orientation;
}

Map* Unit::GetMap() const
{
    return m_Map;
}

void Unit::SetName(const std::string & p_Name)
{
    m_Name = p_Name;
}

void Unit::SetLevel(const uint8 & p_Level)
{
    m_Level = p_Level;
}

void Unit::SetSkinID(const uint8 & p_SkinID)
{
    m_SkinID = p_SkinID;
}

void Unit::SetMapID(const uint16 & p_MapID)
{
    m_MapID = p_MapID;
}

void Unit::SetPosX(const uint32 & p_PosX)
{
    m_PosX = p_PosX;
}

void Unit::SetPoxY(const uint32 & p_PosY)
{
    m_PosY = p_PosY;
}

void Unit::SetOrientation(const uint8 & p_Orientation)
{
    m_Orientation = p_Orientation;
}


void Unit::SetMap(Map* p_Map)
{
    m_Map = p_Map;
}