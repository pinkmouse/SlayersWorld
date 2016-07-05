#pragma once

#include "Case.hpp"
#include "../Define.hpp"
#include <string>
#include <vector>

class Map
{
public:
	Map();
	~Map();
	bool InitializeMap(const std::string &);
	Case* GetCase(uint16);

private:
	uint16 m_ID;
	uint16 m_SizeX;
	uint16 m_SizeY;

	std::vector<Case*>	m_ListCase;

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

