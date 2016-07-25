#pragma once
#include "Map.hpp"
#include <map>
#include <SFML/System/Time.hpp>

class MapManager
{
public:
	MapManager();
	~MapManager();
	bool InitializeMaps();
    Map* GetMap(uint16) const;
    void Update(sf::Time);

private:
    std::map<uint16, Map*> m_MapList;
};

