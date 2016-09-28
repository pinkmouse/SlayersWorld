#include "Creature.hpp"



Creature::Creature(int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_Health, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation) :
    Unit(p_ID, TypeUnit::CREATURE)
{
    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_MovementHandler->SetPosX(p_PosX);
    m_MovementHandler->SetPosY(p_PosY);
    m_MovementHandler->SetOrientation(p_Orientation);
    m_Health = p_Health;
    SetMapID(p_MapID);
    m_MovementHandler->SetSpeed(0.5f);
}


Creature::~Creature()
{
}
