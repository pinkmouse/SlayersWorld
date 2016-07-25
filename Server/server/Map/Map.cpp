#include <cstdio>
#include "Map.hpp"


Map::Map()
{
}


Map::~Map()
{
}

void Map::Update(sf::Time p_Diff)
{
    for (std::vector<Player*>::iterator l_It = m_ListPlayer.begin(); l_It != m_ListPlayer.end(); ++l_It)
    {
        (*l_It)->Update(p_Diff);
    }
}

void Map::AddPlayer(Player* p_Player)
{
    std::vector<Player*>::iterator l_It = std::find(m_ListPlayer.begin(), m_ListPlayer.end(), p_Player);
    if (l_It != m_ListPlayer.end())
        return;

    m_ListPlayer.push_back(p_Player);
}

void Map::RemovePlayer(Player* p_Player)
{
    std::vector<Player*>::iterator l_It = std::find(m_ListPlayer.begin(), m_ListPlayer.end(), p_Player);
    if (l_It != m_ListPlayer.end())
        m_ListPlayer.erase(l_It);
}

bool Map::InitializeMap(const std::string & p_FileName)
{
	FILE * l_File;
	std::string l_MapPath = MAP_PATH + p_FileName;

	l_File = fopen(l_MapPath.c_str(), "r");
	if (l_File == nullptr)
	{
		printf("Map File %s dnot open", l_MapPath.c_str());
		return false;
	}

	t_MapParam l_Param;
	fread(&l_Param, sizeof(l_Param), 1, l_File);
	m_ID = l_Param.l_ID;
	m_SizeX = l_Param.l_Size[0];
	m_SizeY = l_Param.l_Size[1];

	/// Read Map
	for (uint16 i = 0; i < (m_SizeX * m_SizeY); ++i)
	{
		t_Case l_FluxCase;
		fread(&l_FluxCase, sizeof(l_FluxCase), 1, l_File);
		Case* l_Case = new Case(i, i % m_SizeX, i / m_SizeX);

		bool l_Block = l_FluxCase.l_Block;
		if (l_Block)
			l_Case->SetBlock(true);
		m_ListCase.push_back(l_Case);
	}

	return true;
}
