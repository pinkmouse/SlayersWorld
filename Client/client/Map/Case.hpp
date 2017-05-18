#pragma once
#include "../Define.hpp"
#include "../Entities/WorldObject.hpp"
#include "../Entities/DynamicObject.hpp"
#include <array>

class Case : public WorldObject
{
public:
	Case(uint32, uint16, uint16);
	~Case();
	void SetBlock(bool);
	uint8 GetMaxTileLevel();
	void SetTile(uint8, int16);
	int16 GetTile(uint8) const;
    bool IsBlocking() const;
    void AddDynamicOject(DynamicObject*);
    void RemoveDynamicOject(DynamicObject*);

private:
	bool m_Block;
	uint32 m_ID;
	std::array<int16, MAX_TILE_LEVEL> m_TileList;
    std::vector<DynamicObject*> m_DynamicObjectList;
};

