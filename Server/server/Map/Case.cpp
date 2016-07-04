#include "Case.hpp"


Case::Case(uint16 p_ID, uint16 p_X, uint16 p_Y) :
	m_ID(p_ID),
	m_X(p_X),
	m_Y(m_Y),
	m_Block(false)
{
}


Case::~Case()
{
}

void Case::SetBlock(bool p_Block)
{
	m_Block = p_Block;
}