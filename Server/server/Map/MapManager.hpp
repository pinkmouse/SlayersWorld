#pragma once
#include "Map.hpp"
#include "MapTemplate.hpp"
#include <map>
#include <SFML/System/Time.hpp>

class MapManager
{
public:
	MapManager();
	~MapManager();
	bool InitializeMapsTemplate();
    Map* LaunchMap(uint16);
    bool LaunchWorldsMap();
    void AddMapTemplate(MapTemplate*);
    Map* GetMap(uint16, uint16);
    MapTemplate* GetMapTemplate(uint16);
    void Update(sf::Time);
    bool IsOnline(TypeUnit, uint16);
    Player* GetPlayer(uint16);
    std::vector<Player*> GetAllPlayers();
    int32 StartInstance(uint16);

private:
    std::map<uint16, std::map<uint16, Map*> > m_MapList;
    std::map<uint16, MapTemplate*> m_MapListTemplate;
};