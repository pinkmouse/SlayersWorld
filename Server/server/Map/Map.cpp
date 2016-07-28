#include <cstdio>
#include "Map.hpp"


Map::Map()
{
}


Map::~Map()
{
}

uint16 Map::GetSquareID(uint16 p_X, uint16 p_Y) const
{
    uint16 l_DrawingSquareID = 0;

    /// Total square width
    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / SIZE_SENDING_SQUARE);

    uint16 l_XSquare = (uint16)ceil((p_X / TILE_SIZE_X) / SIZE_SENDING_SQUARE);
    uint16 l_YSquare = (uint16)ceil((p_Y / TILE_SIZE_Y) / SIZE_SENDING_SQUARE);

    //printf("Calc %d %d %d", l_TotalSquareWidth, l_YSquare, l_XSquare);
    return (l_TotalSquareWidth * l_YSquare) + l_XSquare;
}

void Map::Update(sf::Time p_Diff)
{
    for (std::vector<Player*>::iterator l_It = m_ListPlayer.begin(); l_It != m_ListPlayer.end(); ++l_It)
    {
        Player* l_Player = (*l_It);

        if (l_Player == nullptr)
            continue;

        l_Player->Update(p_Diff);
        if (l_Player->GetSquareID() != GetSquareID(l_Player->GetPosX(), l_Player->GetPosY()))
            ChangeSquare(l_Player);
    }
}

uint16 Map::ChangeSquare(Unit* p_Unit)
{
    Square* l_OldSquare = &m_ListSquare[p_Unit->GetSquareID()];
    l_OldSquare->RemoveUnit(p_Unit);

    uint16 l_SquareId = GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY());
    Square* l_NewSquare = &m_ListSquare[l_SquareId];
    l_NewSquare->AddUnit(p_Unit);
    p_Unit->SetSquareID(l_SquareId);

    printf("Pass from square %d to square %d\n", p_Unit->GetSquareID(), l_SquareId);
    return l_SquareId;
}

void Map::AddUnit(Unit* p_Unit)
{
    if (p_Unit->GetType() == TypeUnit::PLAYER)
    {
        std::vector<Player*>::iterator l_It = std::find(m_ListPlayer.begin(), m_ListPlayer.end(), p_Unit);
        if (l_It != m_ListPlayer.end())
            return;

        Player* l_Player = p_Unit->ToPlayer();

        if (l_Player == nullptr)
            return;

        m_ListPlayer.push_back(l_Player);
    }
    else
    {
        std::vector<Unit*>::iterator l_It = std::find(m_ListUnit.begin(), m_ListUnit.end(), p_Unit);
        if (l_It != m_ListUnit.end())
            return;

        m_ListUnit.push_back(p_Unit);
    }

    /// Add to square
    uint16 l_SquareId = GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY());
    AddToSquare(p_Unit, l_SquareId);
}

void Map::RemoveUnit(Unit* p_Unit)
{
    if (p_Unit->GetType() == TypeUnit::PLAYER)
    {
        std::vector<Player*>::iterator l_It = std::find(m_ListPlayer.begin(), m_ListPlayer.end(), p_Unit);
        if (l_It != m_ListPlayer.end())
            m_ListPlayer.erase(l_It);
    }
    else
    {
        std::vector<Unit*>::iterator l_It = std::find(m_ListUnit.begin(), m_ListUnit.end(), p_Unit);
        if (l_It != m_ListUnit.end())
            m_ListUnit.erase(l_It);
    }

    /// Remove from square
    RemoveFromSquare(p_Unit);
}

void Map::AddToSquare(Unit* p_Unit, uint16 p_SquareID)
{
    Square* l_Square = &m_ListSquare[p_SquareID];
    p_Unit->SetSquareID(p_SquareID);

    if (l_Square != nullptr)
        l_Square->AddUnit(p_Unit);
}

void Map::RemoveFromSquare(Unit* p_Unit)
{
    uint16 l_SquareId = p_Unit->GetSquareID();
    Square* l_Square = &m_ListSquare[l_SquareId];

    if (l_Square != nullptr)
        l_Square->RemoveUnit(p_Unit);
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

    /// Initialize Squares
   /* uint16 l_TotalSquareWidth = (uint16)ceil(m_SizeX / SIZE_SENDING_SQUARE);
    uint16 l_TotalSquareHeight = (uint16)ceil(m_SizeY / SIZE_SENDING_SQUARE);

    if (!l_TotalSquareWidth || !l_TotalSquareHeight)
        return false;

    uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

    for (uint16 i = 0; i < l_TotalSquare; ++i)
    {
        std::vector<Unit*>* l_Square = new std::vector<Unit*>();
        m_ListSquare[i] = l_Square;
        printf("--> %d\n", i);
    }*/

	return true;
}

std::vector<Square*> Map::GetSquareSet(uint16 p_SquareID)
{
    std::vector<Square*> l_SquareSet;
    l_SquareSet.push_back(&m_ListSquare[p_SquareID]);

    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / SIZE_SENDING_SQUARE);
    uint16 l_TotalSquareHeight = (uint16)ceil((float)m_SizeY / SIZE_SENDING_SQUARE);

    if (!l_TotalSquareWidth || !l_TotalSquareHeight)
        return l_SquareSet;

    uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

    uint16 l_IDReal = p_SquareID + 1;
    //printf("--> ID = %d, %d, %d, %d\n", p_ID, l_TotalSquareWidth, l_TotalSquareHeight, m_SizeY);
    if (l_IDReal - l_TotalSquareWidth > 0) ///< Top Center
    {
        //printf("1\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth]);
    }
    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Bottom Center
    {
        //printf("2\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth]);
    }

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Center
    {
        //printf("3\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - 1]);
    }
    if (l_IDReal % l_TotalSquareWidth > 0) ///< right Center
    {
        //printf("4\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + 1]);
    }

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Left Top
    {
        //printf("5\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth - 1]);
    }
    if (l_IDReal % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Right Top
    {
        //printf("6\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth + 1]);
    }

    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare && (l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Bottom
    {
        //printf("7\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth - 1]);
    }
    if (l_IDReal % l_TotalSquareWidth > 0 && l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Right Bottom
    {
        //printf("8\n");
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth + 1]);
    }

    return l_SquareSet;
}