#include "Case.hpp"

Case::Case(uint16 p_ID, uint16 p_X, uint16 p_Y) :
	m_ID(p_ID),
	m_X(p_X),
	m_Y(p_Y),
	m_Block(false)
{
	for (uint8 i = 0; i < m_TileList.size(); ++i)
		m_TileList[i] = -1;
}

Case::~Case()
{
}

uint16 Case::GetPosX() const
{
	return m_X;
}

uint16 Case::GetPosY() const
{
	return m_Y;
}

int16 Case::GetTile(uint8 p_Level) const
{
	return m_TileList[p_Level];
}

bool Case::IsBlocking() const
{
    return m_Block;
}

void Case::SetTile(uint8 p_Level, int16 p_Value)
{
	m_TileList[p_Level] = p_Value;
}

void Case::SetBlock(bool p_Block)
{
	m_Block = p_Block;
}

uint8 Case::GetMaxTileLevel()
{
	return (uint8)m_TileList.size();
}
