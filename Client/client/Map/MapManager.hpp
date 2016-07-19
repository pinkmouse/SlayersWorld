#pragma once

#include <map>
#include "Map.hpp"
#include <vector>
#include "../Define.hpp"
#include "../Graphics/TileSprite.hpp"

class MapManager
{
public:
	MapManager();
	~MapManager();
	void InitializeMaps();
    bool LoadMap(uint16);
	Map* GetActualMap() const;
	bool HasMap();

    void SetPosX(uint32);
    void SetPosY(uint32);
    uint32 GetPosX() const;
    uint32 GetPosY() const;

private:
	Map* m_ActualMap;
    uint32 m_PosX;
    uint32 m_PosY;
	std::map <uint16, std::string> m_MapsPath;
};

