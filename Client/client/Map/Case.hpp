#pragma once
#include "../Define.hpp"
#include "../Entities/WorldObject.hpp"
#include <array>

class Case : public WorldObject
{
public:
	Case(uint16, uint16, uint16);
	~Case();
	void SetBlock(bool);
	uint8 GetMaxTileLevel();
	void SetTile(uint8, int16);
	int16 GetTile(uint8) const;
    bool IsBlocking() const;

private:
	bool m_Block;
	uint16 m_ID;
	std::array<int16, MAX_TILE_LEVEL> m_TileList;
};

