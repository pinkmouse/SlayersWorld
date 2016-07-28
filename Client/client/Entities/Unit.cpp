#include "Unit.hpp"


Unit::Unit(uint16 p_ID)
{
    m_Name = "";
    m_MapID = 0;
    m_PosX = 0;
    m_PosY = 0;
    m_MovementHandler = new MovementHandler();
    m_Orientation = Orientation::Down;
    m_Type = TypeUnit::CREATURE;
    m_ID = p_ID;
}

Unit::Unit(uint16 p_ID, TypeUnit p_Type)
{
    m_Name = "";
    m_MapID = 0;
    m_PosX = 0;
    m_PosY = 0;
    m_MovementHandler = new MovementHandler();
    m_Orientation = Orientation::Down;
    m_Type = p_Type;
    m_ID = p_ID;
}


Unit::~Unit()
{
}

MovementHandler* Unit::GetMovementHandler()
{
    return m_MovementHandler;
}

void Unit::Update(sf::Time p_Diff)
{
    m_MovementHandler->Update(p_Diff);
    m_PosX = m_MovementHandler->GetPosX();
    m_PosY = m_MovementHandler->GetPosY();
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

Orientation Unit::GetOrientation() const
{
    return m_Orientation;
}

TypeUnit Unit::GetType() const
{
    return m_Type;
}

Player* Unit::ToPlayer()
{
    if (m_Type == TypeUnit::PLAYER)
        return  reinterpret_cast<Player*>(this);
    else
        return nullptr;
}

uint16 Unit::GetID() const
{
    return m_ID;
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

void Unit::SetPosY(const uint32 & p_PosY)
{
    m_PosY = p_PosY;
    m_MovementHandler->SetPosY(p_PosY);
}

void Unit::SetOrientation(const Orientation & p_Orientation)
{
    m_Orientation = p_Orientation;
    m_MovementHandler->SetOrientation((Orientation)p_Orientation);
}