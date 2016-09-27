#include <cstdio>
#include "Map.hpp"
#include "../World/WorldSocket.hpp"


Map::Map()
{
    m_SizeX = 0;
    m_SizeY = 0;
}


Map::~Map()
{
}

uint16 Map::GetSizeX() const
{
    return m_SizeX;
}

uint16 Map::GetSizeY() const
{
    return m_SizeY;
}

Case* Map::GetCase(uint16 p_ID) const
{
    return m_ListCase[p_ID];
}

Case* Map::GetCase(uint32 p_PosX, uint32 p_PosY) const
{
    return m_ListCase[(uint16)((p_PosY / TILE_SIZE) * m_SizeX) + (p_PosX / TILE_SIZE)];
}

uint16 Map::GetSquareID(uint16 p_X, uint16 p_Y) const
{
    uint16 l_DrawingSquareID = 0;

    /// Total square width
    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / GRID_SIZE);

    uint16 l_XSquare = (uint16)ceil((p_X / TILE_SIZE) / GRID_SIZE);
    uint16 l_YSquare = (uint16)ceil((p_Y / TILE_SIZE) / GRID_SIZE);

    //printf("Calc %d %d %d", l_TotalSquareWidth, l_YSquare, l_XSquare);
    return (l_TotalSquareWidth * l_YSquare) + l_XSquare;
}

Unit* Map::GetUnit(TypeUnit p_TypeID, uint16 p_UnitID)
{
    return m_ListUnitZone[p_TypeID][p_UnitID];

}

void Map::Update(sf::Time p_Diff)
{
    for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_MapListUnit : m_ListUnitZone)
    {
        TypeUnit l_Type = l_MapListUnit.first;
        for (std::pair<uint16, Unit*> l_UnitPair : l_MapListUnit.second)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            if (l_Type == TypeUnit::PLAYER)
                l_Unit->ToPlayer()->Update(p_Diff);
            else if (l_Type == TypeUnit::CREATURE)
                l_Unit->ToCreature()->Update(p_Diff);
            else
                l_Unit->Update(p_Diff);

            if (l_Unit->GetSquareID() != GetSquareID(l_Unit->GetPosX(), l_Unit->GetPosY()))
                ChangeSquare(l_Unit);
        }
    }
}

uint16 Map::ChangeSquare(Unit* p_Unit)
{
    /// Send the new Unit to Player entering
    if (p_Unit->GetType() == TypeUnit::PLAYER)
        p_Unit->ToPlayer()->UpdateNewSquares(p_Unit->GetSquareID(), GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY()));

    /// Send the new Unit entering for the Player already in square
    UpdateForPlayersInNewSquare(p_Unit);

    /// Remove from old square
    Square* l_OldSquare = &m_ListSquare[p_Unit->GetSquareID()];
    l_OldSquare->RemoveUnit(p_Unit);

    /// Add to new square
    uint16 l_SquareId = GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY());
    AddToSquare(p_Unit, l_SquareId);

    return l_SquareId;
}

void Map::UpdateForPlayersInNewSquare(Unit* p_Unit, bool p_UpdateAll)
{
    std::vector<uint16> l_OldSquareSet;
    if (!p_UpdateAll)
        l_OldSquareSet = GetSquareSetID(p_Unit->GetSquareID());
    std::vector<uint16> l_NewSquareSet = GetSquareSetID(GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY()));

    std::vector<uint16> l_DiffSquareSet;
    for (uint16 l_Id : l_NewSquareSet)
    {
        std::vector<uint16>::iterator l_It = std::find(l_OldSquareSet.begin(), l_OldSquareSet.end(), l_Id);

        if (l_It == l_OldSquareSet.end())
            l_DiffSquareSet.push_back(l_Id);
    }

    for (uint16 l_Id : l_DiffSquareSet)
    {
        Square* l_Square = GetSquare(l_Id);

        if (l_Square == nullptr)
            continue;

        std::map<uint16, Unit*>* l_SquareList = l_Square->GetList(TypeUnit::PLAYER);

        for (std::pair<uint16, Unit*> l_UnitPair : *l_SquareList)
        {
            Player* l_Player = l_UnitPair.second->ToPlayer();
            if (l_Player == nullptr)
                continue;

            WorldSocket* l_Session = l_Player->GetSession();
            if (l_Session == nullptr)
                continue;

            l_Session->SendUnitCreate(p_Unit->GetType(), p_Unit->GetID(), p_Unit->GetName(), p_Unit->GetLevel(), p_Unit->GetHealth(), p_Unit->GetSkinID(), p_Unit->GetMapID(), p_Unit->GetPosX(), p_Unit->GetPosY(), p_Unit->GetOrientation(), p_Unit->IsInMovement());
        }
    }
}

Unit* Map::GetCloserUnit(Unit const* p_Unit, float p_Range /*= 2.0f*/, bool p_OnlyInLife /*= flase*/, bool p_InFront /*= true*/)
{
    /// TODO : QuadTree
    std::vector<Square*> l_Grid = GetSquareSet(p_Unit->GetSquareID());
    Unit* l_CloserUnit = nullptr;

    for (uint8 i = 0; i < l_Grid.size(); ++i)
    {
        Square* l_Square = l_Grid[i];

        std::map<TypeUnit, std::map<uint16, Unit*>>* l_SquareList = l_Square->GetList();

        for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_SquareMap : *l_SquareList)
        {
            for (std::pair<uint16, Unit*> l_SquareList : l_SquareMap.second)
            {
                Unit* l_Unit = l_SquareList.second;

                if (l_Unit == nullptr)
                    continue;

                if (l_Unit == p_Unit)
                    continue;

                if (p_OnlyInLife && l_Unit->IsDeath())
                    continue;

                float l_Dist = p_Unit->GetDistance(l_Unit);

                if (l_Dist > p_Range)
                    continue;

                if (p_InFront && !p_Unit->IsInFront(l_Unit))
                    continue;

                if (l_CloserUnit == nullptr)
                    l_CloserUnit = l_Unit;
                else if (l_CloserUnit != nullptr && l_Dist < p_Unit->GetDistance(l_CloserUnit))
                    l_CloserUnit = l_Unit;
            }
        }
    }
    return l_CloserUnit;
}

void Map::AddUnit(Unit* p_Unit)
{
    p_Unit->SetMap(this);
    m_ListUnitZone[p_Unit->GetType()][p_Unit->GetID()] = p_Unit;

    /// Add to square
    uint16 l_SquareId = GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY());
    printf("Add to square %d\n", l_SquareId);
    AddToSquare(p_Unit, l_SquareId);
    UpdateForPlayersInNewSquare(p_Unit, true);
}

void Map::RemoveUnit(Unit* p_Unit)
{
    m_ListUnitZone[p_Unit->GetType()].erase(p_Unit->GetID());

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
		printf("Map File %s did not open\n", l_MapPath.c_str());
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

Square* Map::GetSquare(uint16 p_SquareID)
{
    return &m_ListSquare[p_SquareID];
}

std::vector<Square*> Map::GetSquareSet(uint16 p_SquareID)
{
    std::vector<Square*> l_SquareSet;
    Square* l_Square = GetSquare(p_SquareID);
    l_SquareSet.push_back(l_Square);

    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / GRID_SIZE);
    uint16 l_TotalSquareHeight = (uint16)ceil((float)m_SizeY / GRID_SIZE);

    if (!l_TotalSquareWidth || !l_TotalSquareHeight)
        return l_SquareSet;

    uint16 l_TotalSquare = l_TotalSquareWidth * l_TotalSquareHeight;

    uint16 l_IDReal = p_SquareID + 1;
    if (l_IDReal - l_TotalSquareWidth > 0) ///< Top Center
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth]);
    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Bottom Center
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth]);

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Center
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - 1]);
    if (l_IDReal % l_TotalSquareWidth > 0) ///< right Center
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + 1]);

    if ((l_IDReal - 1) % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Left Top
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth - 1]);
    if (l_IDReal % l_TotalSquareWidth > 0 && (l_IDReal - l_TotalSquareWidth - 1 >= 0)) ///< Right Top
        l_SquareSet.push_back(&m_ListSquare[p_SquareID - l_TotalSquareWidth + 1]);

    if (l_IDReal + l_TotalSquareWidth <= l_TotalSquare && (l_IDReal - 1) % l_TotalSquareWidth > 0) ///< Left Bottom
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth - 1]);
    if (l_IDReal % l_TotalSquareWidth > 0 && l_IDReal + l_TotalSquareWidth <= l_TotalSquare) ///< Right Bottom
        l_SquareSet.push_back(&m_ListSquare[p_SquareID + l_TotalSquareWidth + 1]);

    return l_SquareSet;
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