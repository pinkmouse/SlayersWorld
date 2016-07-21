#pragma once

#include <map>
#include "Map.hpp"
#include <vector>
#include "../Define.hpp"
#include "../Graphics/TileSprite.hpp"
#include "../Entities/Player.hpp"
#include "../World/Events/Events.hpp"

class MapManager
{
public:
	MapManager(Events*);
	~MapManager();
	void InitializeMaps();
    bool LoadMap(uint16);
	Map* GetActualMap() const;
	bool HasMap();

    void SetPosX(uint32);
    void SetPosY(uint32);
    uint32 GetPosX() const;
    uint32 GetPosY() const;

    void SetMainPlayer(Player*);
    Player* GetMainPlayer();

private:
	Map* m_ActualMap;
    Player* m_MainPlayer;
    uint32 m_PosX;
    uint32 m_PosY;
	std::map<uint16, std::string> m_MapsPath;

    Events* m_Events;
};

