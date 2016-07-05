#pragma once
#include "../Define.hpp"
#include <array>

class Case
{
public:
	Case(uint16, uint16, uint16);
	~Case();
	void SetBlock(bool);
	uint8 GetMaxTileLevel();
	void SetTile(uint8, int16);
	int16 GetTile(uint8) const;
	uint16 GetPosX() const;
	uint16 GetPosY() const;

private:
	bool m_Block;
	uint16 m_ID;
	uint16 m_X;
	uint16 m_Y;
	std::array<int16, MAX_TILE_LEVEL> m_TileList;
};

