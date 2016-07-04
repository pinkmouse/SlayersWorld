#pragma once

#include <map>
#include "Map.hpp"
#include "../Define.hpp"

class MapManager
{
public:
	MapManager();
	~MapManager();
	void InitializeMaps();
	void LoadMap(uint8);

private:
	Map* m_ActualMap;
	std::map <uint8, std::string> m_MapsPath;
};

