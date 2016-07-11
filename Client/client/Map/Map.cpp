#include <cstdio>
#include <cmath>
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

std::vector<Case*> Map::GetSquare(uint16 p_ID)
{
	return m_MapListCase[p_ID];
}

std::vector<std::vector<Case*>> Map::GetSquareZone(uint16 p_ID)
{
	std::vector<std::vector<Case*>> l_SquareZone;
	uint16 l_TotalSquareWidth = ceil((float)m_SizeX / SIZE_DRAWING_SQUARE);
	uint16 l_TotalSquareHeight = ceil((float)m_SizeY / SIZE_DRAWING_SQUARE);
	uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

	l_SquareZone.push_back(GetSquare(p_ID));
	uint16 l_IDReal = p_ID + 1;
	printf("--> ID = %d, %d, %d, %d\n", p_ID, l_TotalSquareWidth, l_TotalSquareHeight, m_SizeY);
	if (l_IDReal - l_TotalSquareWidth > 0)
	{
		printf("1\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth)); ///< Top Center
	}
	if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare)
	{
		printf("2\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth)); ///< Bottom Center
	}

	if ((l_IDReal - 1) % l_TotalSquareWidth > 0)
	{
		printf("3\n");
		l_SquareZone.push_back(GetSquare(p_ID - 1)); ///< Left Center
	}
	if (l_IDReal % l_TotalSquareWidth > 0)
	{
		printf("4\n");
		l_SquareZone.push_back(GetSquare(p_ID + 1)); ///< right Center
	}

	if ((l_IDReal - 1) % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0))
	{
		printf("5\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth - 1)); ///< Left Top
	}
	if (l_IDReal % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0))
	{
		printf("6\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth + 1)); ///< Right Top
	}

	if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare && (l_IDReal - 1) % l_TotalSquareWidth > 0)
	{
		printf("7\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth - 1)); ///< Left Bottom
	}
	if (l_IDReal % l_TotalSquareWidth > 0 && l_IDReal + l_TotalSquareWidth <= l_TotalSquare)
	{
		printf("8\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth + 1)); ///< Right Bottom
	}

	return l_SquareZone;
}

Case* Map::GetCase(uint16 p_ID) const
{
	return m_ListCase[p_ID];
}

uint16 Map::GetSizeX() const
{
	return m_SizeX;
}

uint16 Map::GetSizeY() const
{
	return m_SizeY;
}

bool Map::IsValidMap()
{
	if (m_SizeX <= 0 || m_SizeY <= 0)
		return false;

	if (m_MapListCase.size() <= 0)
		return false;

	return true;
}

uint16 Map::GetSquareID(uint16 p_X, uint16 p_Y) const
{
	uint16 l_DrawingSquareID = 0;

	/// Total square width
	uint16 l_TotalSquareWidth = ceil((float)m_SizeX / SIZE_DRAWING_SQUARE);

	uint16 l_XSquare = ceil(p_X / SIZE_DRAWING_SQUARE);
	uint16 l_YSquare = ceil(p_Y / SIZE_DRAWING_SQUARE);

	printf("Calc %d %d %d", l_TotalSquareWidth, l_YSquare, l_XSquare);
	return (l_TotalSquareWidth * l_YSquare) + l_XSquare;
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

		uint16 l_DrawingSquareID = GetSquareID(l_Case->GetPosX(), l_Case->GetPosY());
		m_ListCase.push_back(l_Case);
		m_MapListCase[l_DrawingSquareID].push_back(l_Case);
	}
	fclose(l_File);

	return true;
}