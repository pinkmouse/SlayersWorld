#include <cstdio>
#include "MapManager.hpp"


MapManager::MapManager()
{
}


MapManager::~MapManager()
{
}

bool MapManager::InitializeMaps()
{
	Map* l_Map = new Map();
	if (!l_Map->InitializeMap("map00.map"))
		return false;
	return true;
}