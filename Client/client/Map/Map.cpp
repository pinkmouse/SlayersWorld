#include <cstdio>
#include <cmath>
#include "../Global.hpp"
#include "Map.hpp"

Map::Map(uint16 p_MapID)
{
	m_ID = p_MapID;
	m_SizeX = 0;
	m_SizeY = 0;
}


Map::~Map()
{
    for (Case* l_Case : m_ListCase)
        delete l_Case;
    for (std::pair< TypeUnit, std::map<uint16, Unit*> > l_ListUnit : m_ListUnitZone)
    {
        for (std::pair< uint16, Unit* > l_Unit : l_ListUnit.second)
        {
            if (l_Unit.second == g_Player)
                continue;
            delete l_Unit.second;
        }
    }
}

void Map::Update(sf::Time p_Diff)
{
    for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_MapListUnit : m_ListUnitZone)
    {
        for (std::pair<uint16, Unit*> l_UnitPair : l_MapListUnit.second)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            if (g_Player == nullptr)
                return;

            if (!g_Player->IsInRayVisible(l_Unit))
            {
                m_ListUnitZone[l_Unit->GetType()].erase(l_Unit->GetID());
            }
            else
                l_Unit->Update(p_Diff);
        }
    }
}

std::map<TypeUnit, std::map<uint16, Unit*>>* Map::GetListUnitZone()
{
    return &m_ListUnitZone;
}

void Map::AddUnit(Unit* p_Unit)
{
    m_ListUnitZone[p_Unit->GetType()][p_Unit->GetID()] = p_Unit;
}

void Map::RemoveUnit(Unit* p_Unit)
{
    m_ListUnitZone[p_Unit->GetType()].erase(p_Unit->GetID());
}

Unit* Map::GetUnit(TypeUnit p_TypeID, uint16 p_UnitID)
{
    return m_ListUnitZone[p_TypeID][p_UnitID];
}

void Map::MoveUnitToDirection(TypeUnit p_TypeID, uint16 p_UnitID, Position p_Pos, uint8 p_Direction)
{
    Unit* l_Unit = GetUnit(p_TypeID, p_UnitID);

    if (l_Unit == nullptr)
        return;

    l_Unit->GetMovementHandler()->AddMovementToStack(eActionType::Go, p_Pos, (Orientation)p_Direction);
    /*l_Unit->GetMovementHandler()->StartMovement((Orientation)p_Direction);
    l_Unit->SetOrientation((Orientation)p_Direction);*/
}

std::vector<Case*> Map::GetSquare(uint16 p_ID)
{
	return m_MapListCase[p_ID];
}

std::vector<std::vector<Case*>> Map::GetSquareZone(uint16 p_ID)
{
	std::vector<std::vector<Case*>> l_SquareZone;
	uint16 l_TotalSquareWidth = ceil((float)m_SizeX / GRID_SIZE);
	uint16 l_TotalSquareHeight = ceil((float)m_SizeY / GRID_SIZE);
	uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

	l_SquareZone.push_back(GetSquare(p_ID));
	uint16 l_IDReal = p_ID + 1;
	//printf("--> ID = %d, %d, %d, %d\n", p_ID, l_TotalSquareWidth, l_TotalSquareHeight, m_SizeY);
	if (l_IDReal - l_TotalSquareWidth > 0)
	{
		//printf("1\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth)); ///< Top Center
	}
	if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare)
	{
		//printf("2\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth)); ///< Bottom Center
	}

	if ((l_IDReal - 1) % l_TotalSquareWidth > 0)
	{
		//printf("3\n");
		l_SquareZone.push_back(GetSquare(p_ID - 1)); ///< Left Center
	}
	if (l_IDReal % l_TotalSquareWidth > 0)
	{
		//printf("4\n");
		l_SquareZone.push_back(GetSquare(p_ID + 1)); ///< right Center
	}

	if ((l_IDReal - 1) % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0))
	{
		//printf("5\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth - 1)); ///< Left Top
	}
	if (l_IDReal % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0))
	{
		//printf("6\n");
		l_SquareZone.push_back(GetSquare(p_ID - l_TotalSquareWidth + 1)); ///< Right Top
	}

	if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare && (l_IDReal - 1) % l_TotalSquareWidth > 0)
	{
		//printf("7\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth - 1)); ///< Left Bottom
	}
	if (l_IDReal % l_TotalSquareWidth > 0 && l_IDReal + l_TotalSquareWidth <= l_TotalSquare)
	{
		//printf("8\n");
		l_SquareZone.push_back(GetSquare(p_ID + l_TotalSquareWidth + 1)); ///< Right Bottom
	}

	return l_SquareZone;
}

Case* Map::GetCase(uint16 p_ID) const
{
	return m_ListCase[p_ID];
}

Case* Map::GetCase(uint32 p_PosX, uint32 p_PosY) const
{
    return m_ListCase[(uint16)((p_PosY / TILE_SIZE) * m_SizeX) + (p_PosX / TILE_SIZE)];
}

uint16 Map::GetSizeX() const
{
	return m_SizeX;
}

uint16 Map::GetSizeY() const
{
	return m_SizeY;
}

uint16 Map::GetID() const
{
    return m_ID;
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
	uint16 l_TotalSquareWidth = ceil((float)m_SizeX / GRID_SIZE);

	uint16 l_XSquare = ceil(p_X / GRID_SIZE);
	uint16 l_YSquare = ceil(p_Y / GRID_SIZE);
	//printf("Calc %d %d %d", l_TotalSquareWidth, l_YSquare, l_XSquare);
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
    int l_NbCase = m_SizeX * m_SizeY;
    t_Case *Buffer = (t_Case*)malloc(sizeof(t_Case) * l_NbCase);
    fread(Buffer, sizeof(*Buffer), l_NbCase, l_File);

    for (uint16 i = 0; i < (m_SizeX * m_SizeY); ++i)
    {
        Case* l_Case = new Case(i, i % m_SizeX, i / m_SizeX);
        l_Case->SetMapID(m_ID);

        for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
            l_Case->SetTile(j, Buffer[i].l_TabTileNb[j]);
        l_Case->SetBlock(Buffer[i].l_Block);

        uint16 l_DrawingSquareID = GetSquareID(l_Case->GetPosXCase(), l_Case->GetPosYCase());

        m_ListCase.push_back(l_Case);
        m_MapListCase[l_DrawingSquareID].push_back(l_Case);
    }

    /// Following code are not working for big map because of buffer
	/*for (uint16 i = 0; i < (m_SizeX * m_SizeY); ++i)
	{
		t_Case l_FluxCase;
        int l_NbCase = m_SizeX * m_SizeY;
		fread(&l_FluxCase, sizeof(l_FluxCase), 1, l_File);

		Case* l_Case = new Case(i, i % m_SizeX, i / m_SizeX);
        l_Case->SetMapID(m_ID);
        if (i > 35550)
            printf("----- Case %d tile = %i\n", i, l_FluxCase.l_TabTileNb[0]);

		for (int j = 0; j < l_Case->GetMaxTileLevel(); ++j)
			l_Case->SetTile(j, l_FluxCase.l_TabTileNb[j]);
		l_Case->SetBlock(l_FluxCase.l_Block);

		uint16 l_DrawingSquareID = GetSquareID(l_Case->GetPosXCase(), l_Case->GetPosYCase());

		m_ListCase.push_back(l_Case);
		m_MapListCase[l_DrawingSquareID].push_back(l_Case);
	}*/
	fclose(l_File);

	return true;
}

std::vector<uint16> Map::GetSquareSetID(uint16 p_SquareID)
{
    std::vector<uint16> l_SquareSet;
    l_SquareSet.push_back(p_SquareID);

    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / GRID_SIZE);
    uint16 l_TotalSquareHeight = (uint16)ceil((float)m_SizeY / GRID_SIZE);

    if (!l_TotalSquareWidth || !l_TotalSquareHeight)
        return l_SquareSet;

    uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

    uint16 l_IDReal = p_SquareID + 1;
    if (l_IDReal - l_TotalSquareWidth > 0) ///< Top Center
        l_SquareSet.push_back(p_SquareID - l_TotalSquareWidth);
    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Bottom Center
        l_SquareSet.push_back(p_SquareID + l_TotalSquareWidth);

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Center
        l_SquareSet.push_back(p_SquareID - 1);
    if (l_IDReal % l_TotalSquareWidth > 0) ///< right Center
        l_SquareSet.push_back(p_SquareID + 1);

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Left Top
        l_SquareSet.push_back(p_SquareID - l_TotalSquareWidth - 1);
    if (l_IDReal % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Right Top
        l_SquareSet.push_back(p_SquareID - l_TotalSquareWidth + 1);

    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare && (l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Bottom
        l_SquareSet.push_back(p_SquareID + l_TotalSquareWidth - 1);
    if (l_IDReal % l_TotalSquareWidth > 0 && l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Right Bottom
        l_SquareSet.push_back(p_SquareID + l_TotalSquareWidth + 1);

    return l_SquareSet;
}