#include "Case.hpp"


Case::Case(uint16 p_ID, uint16 p_X, uint16 p_Y) :
	m_ID(p_ID),
	m_Block(false)
{
    SetPosX(p_X);
    SetPosY(p_Y);
}

Case::~Case()
{
}

uint16 Case::GetID() const
{
    return m_ID;
}

bool Case::IsBlocking() const
{
    return m_Block;
}

void Case::SetBlock(bool p_Block)
{
	m_Block = p_Block;
}