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
	FILE * l_File;
	l_File = fopen("map/map00.map", "r");
	if (l_File == nullptr)
		printf("File not open");
	return true;
}