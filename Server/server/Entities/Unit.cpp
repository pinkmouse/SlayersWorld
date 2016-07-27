#include "Unit.hpp"
#include "../Map/Map.hpp"


Unit::Unit()
{
    m_Name = "";
    m_MapID = 0;
    m_PosX = 0;
    m_PosY = 0;
    m_MovementHandler = new MovementHandler();
    m_Orientation = Orientation::Down;
    m_Type = TypeUnit::CREATURE;
}

Unit::Unit(TypeUnit p_Type)
{
    m_Type = p_Type;
    m_Name = "";
    m_MapID = 0;
    m_PosX = 0;
    m_PosY = 0;
    m_MovementHandler = new MovementHandler();
    m_Orientation = Orientation::Down;
}

Player* Unit::ToPlayer()
{
    return (Player*)this;
}

Unit::~Unit()
{
    m_MovementHandler->StopMovement();
    m_Map->RemoveUnit(this);
    delete m_MovementHandler;
}

void Unit::Update(sf::Time p_Diff)
{
    if (m_MovementHandler == nullptr)
        return;

    m_MovementHandler->Update(p_Diff);
    m_PosX = m_MovementHandler->GetPosX();
    m_PosY = m_MovementHandler->GetPosY();
}

TypeUnit Unit::GetType() const
{
    return m_Type;
}

MovementHandler* Unit::GetMovementHandler()
{
    return m_MovementHandler;
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
    m_MovementHandler->SetPosX(p_PosX);
}

void Unit::SetPoxY(const uint32 & p_PosY)
{
    m_PosY = p_PosY;
    m_MovementHandler->SetPosY(p_PosY);
}

void Unit::SetOrientation(const Orientation & p_Orientation)
{
    m_Orientation = p_Orientation;
    m_MovementHandler->SetOrientation(p_Orientation);
}

void Unit::SetMap(Map* p_Map)
{
    m_Map = p_Map;
}

uint16 Unit::GetSquareID() const
{
    return m_SquareID;
}

void Unit::SetSquareID(uint16 p_SquareID)
{
    m_SquareID = p_SquareID;
}