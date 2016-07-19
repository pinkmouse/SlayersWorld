#pragma once
#include "Map.hpp"
#include <map>

class MapManager
{
public:
	MapManager();
	~MapManager();
	bool InitializeMaps();
    Map* GetMap(uint16) const;

private:
    std::map<uint16, Map*> m_MapList;
};

