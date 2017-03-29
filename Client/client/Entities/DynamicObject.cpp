#include "DynamicObject.hpp"
#include "../Map/Map.hpp"

DynamicObject::DynamicObject(int32 p_ID, TypeUnit p_Type, std::string p_Name, uint8 p_Level, uint8 p_Health, int16 p_SkinID, uint8 p_SizeX, uint8 p_SizeY, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation) :
    Unit(p_ID, p_Type, p_SizeX, p_SizeY)
{
    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    SetPosX(p_PosX);
    SetPosY(p_PosY);
    m_MovementHandler->SetPosX(p_PosX);
    m_MovementHandler->SetPosY(p_PosY);
    m_MovementHandler->SetOrientation(p_Orientation);
    SetResourceNb(eResourceType::Health, p_Health);
    SetMapID(p_MapID);
    SetSpeed(0.0f);
}

DynamicObject::~DynamicObject()
{
}
