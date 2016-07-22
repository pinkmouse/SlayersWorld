#pragma once

#include "Case.hpp"
#include "../Define.hpp"
#include <string>
#include <map>
#include <vector>
#include "../Entities/Player.hpp"

class Map
{
public:
	Map();
	~Map();
	bool InitializeMap(const std::string &);
	bool IsValidMap();
	
	uint16 GetSquareID(uint16, uint16) const;
	std::vector<Case*> GetSquare(uint16);
	std::vector<std::vector<Case*>> Map::GetSquareZone(uint16);
	Case* GetCase(uint16) const;
	uint16 GetSizeX() const;
	uint16 GetSizeY() const;

    void AddPlayer(Player*);
    void RemovePlayer(Player*);

    std::vector<Player*> GetPlayersInRay(uint32, uint32);

    void Update(sf::Time);

private:
	uint16 m_ID;
	uint16 m_SizeX;
	uint16 m_SizeY;

	std::vector<Case*> m_ListCase;
	std::map<uint16, std::vector<Case*>>	m_MapListCase;

    std::vector<Player*> m_ListPlayerZone;

	struct          t_MapParam
	{
		int l_ID;
		int l_Size[2];
	};

	struct          t_Case
	{
		int l_TabTileNb[4];
		bool l_Block;
	};

};

