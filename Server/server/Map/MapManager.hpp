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
    void UpdateQueues(sf::Time);
    bool IsOnline(TypeUnit, uint16);
    Player* GetPlayer(uint16);
    std::vector<Player*> GetAllPlayers();
    uint16 GetTotalPlayers();
    uint16 GetValidInstanceIDForMap(uint16);
    /* Battleground */
    void AddBGTemplate(BGTemplate*);
    void AddPlayerToQueue(uint16, Player*);


private:
    std::map<uint16, std::map<uint16, Map*> > m_MapList;
    std::map<uint16, MapTemplate*> m_MapListTemplate;
    std::map<uint16, std::pair<BGTemplate*, std::vector<Player*>>> m_BGListTemplate;
    //std::map<uint16, std::vector<Player*> > m_BGListQueue;
};