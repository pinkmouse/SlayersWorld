#include <cstdio>
#include "Map.hpp"

Map::Map()
{
	m_ID = 0;
	m_SizeX = 0;
	m_SizeY = 0;
}


Map::~Map()
{
}

Case* Map::GetCase(uint16 p_ID)
{
	return m_ListCase[p_ID];
}

bool Map::InitializeMap(const std::string & p_FileName)
{
	FILE * l_File;
	std::string l_MapPath = MAP_FOLDER + p_FileName;

	l_File = fopen(l_MapPath.c_str(), "r");
	if (l_File == nullptr)
	{
		printf("Map File %s not open", l_MapPath.c_str());
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
		for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
			l_Case->SetTile(j, l_FluxCase.l_TabTileNb[j]);
		l_Case->SetBlock(l_FluxCase.l_Block);
		printf("PCharge map %d\n", m_ListCase.size());

		m_ListCase.push_back(l_Case);
	}
	return true;
}