#include "Player.hpp"


Player::Player(int32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, Orientation p_Orientation) :
    Unit(p_ID, TypeUnit::PLAYER)
{
    m_Name = p_Name;
    m_Level = p_Level;
    m_SkinID = p_SkinID;
    m_MapID = p_MapID;
    m_PosX = p_PosX;
    m_PosY = p_PosY;
    m_Orientation = p_Orientation;
    m_MovementHandler->SetPosX(p_PosX);
    m_MovementHandler->SetPosY(p_PosY);
    m_MovementHandler->SetOrientation(p_Orientation);
}

Player::~Player()
{
    printf("Erase Player %d:%s", m_ID, m_Name.c_str());
}
