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
    Map* GetMap(uint16);
    void Update(sf::Time);
    bool IsOnline(TypeUnit, uint16);
    Player* GetPlayer(uint16);
    std::vector<Player*> GetAllPlayers();

private:
    std::map<uint16, Map*> m_MapList;
};

