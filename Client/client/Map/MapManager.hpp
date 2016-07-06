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
	void LoadMap(uint8);
	Map* GetActualMap() const;
	bool HasMap();

private:
	Map* m_ActualMap;
	std::map <uint8, std::string> m_MapsPath;
};

