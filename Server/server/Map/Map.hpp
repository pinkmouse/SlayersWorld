#pragma once
#include "../Define.hpp"
#include "Case.hpp"
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>
#include "../Entities/Unit.hpp"
#include "../Entities/Player.hpp"

class Map
{
public:
	Map();
	~Map();
	bool InitializeMap(const std::string &);
    void Update(sf::Time);
    void AddPlayer(Player*);
    void RemovePlayer(Player*);

private:
	uint16 m_ID;
	uint16 m_SizeX;
	uint16 m_SizeY;

	std::vector<Case*>	m_ListCase;
    std::vector<Unit*>  m_ListUnit;
    std::vector<Player*>  m_ListPlayer;

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

