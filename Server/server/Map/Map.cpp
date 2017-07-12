#include <cstdio>
#include "Map.hpp"
#include "../World/WorldSocket.hpp"
#include "../World/PacketDefine.hpp"

Map::Map(uint16 p_InstanceID,  MapTemplate* p_Template)
{
    m_ID = p_Template->GetID();
    m_InstanceID = p_InstanceID;
    m_SizeX = p_Template->GetSizeX();
    m_SizeY = p_Template->GetSizeY();

    std::vector<Case> l_ListCase = (*p_Template->GetListCase());
    uint32 i;
    for (i = 0; i < l_ListCase.size(); i++)
        AddCase(new Case(l_ListCase[i]));

    std::vector<Zone> l_ListZone = (*p_Template->GetListZone());
    for ( i = 0; i < l_ListZone.size(); i++)
        AddZone(new Zone(l_ListZone[i]));

    std::vector<AreaTriggerMapTemplate> l_ListArea = (*p_Template->GetListArea());
    for (i = 0; i < l_ListArea.size(); i++)
    {
        Areatrigger* l_Areatrigger = new Areatrigger(l_ListArea[i].m_ID, this, l_ListArea[i].m_PosX, l_ListArea[i].m_PosY, l_ListArea[i].m_Template);
        AddUnit(l_Areatrigger);
        GetCase(l_ListArea[i].m_CaseNb)->AddDynamicOject(l_Areatrigger);
    }

    std::vector<GobMapTemplate> l_ListGob = (*p_Template->GetListGob());
    for (i = 0; i < l_ListGob.size(); i++)
    {
        GameObject* l_Gob = new GameObject(l_ListGob[i].m_ID, this, l_ListGob[i].m_PosX, l_ListGob[i].m_PosY, l_ListGob[i].m_Template);
        l_Gob->SetGossipList(l_ListGob[i].m_GossipList);
        AddUnit(l_Gob);
        GetCase(l_ListGob[i].m_CaseNb)->AddDynamicOject(l_Gob);
    }

    std::vector<CreatureMapTemplate> l_ListCreature = (*p_Template->GetListCreature());
    for (i = 0; i < l_ListCreature.size(); i++)
    {
        Creature* l_Creature = new Creature(l_ListCreature[i].m_ID, l_ListCreature[i].m_Template, l_ListCreature[i].m_MapID, l_ListCreature[i].m_PosX, l_ListCreature[i].m_PosY);
        l_Creature->SetGossipList(l_ListCreature[i].m_GossipList);
        AddUnit(l_Creature);
    }

    m_GroupManager = nullptr;
}

Map::~Map()
{
    for (Case* l_Case : m_ListCase)
        delete l_Case;
    for (std::pair< TypeUnit, std::map<uint16, Unit*> > l_ListUnit : m_ListUnitZone)
    {
        for (std::pair< uint16, Unit* > l_Unit : l_ListUnit.second)
            delete l_Unit.second;
    }

    for (std::pair< uint16, Zone* > l_Zone : m_ListZone)
    {
        delete l_Zone.second;
    }

    delete m_GroupManager;
}

uint16 Map::GetInstanceID() const
{
    return m_InstanceID;
}

uint16 Map::GetSizeX() const
{
    return m_SizeX;
}

uint16 Map::GetSizeY() const
{
    return m_SizeY;
}

Case* Map::GetCase(uint32 p_ID) const
{
    if (p_ID >= m_ListCase.size())
        return nullptr;

    return m_ListCase[p_ID];
}

uint16 Map::GetID() const
{
    return m_ID;
}

void Map::SetListCase(std::vector<Case*> p_ListCase)
{
    m_ListCase = p_ListCase;
}

Case* Map::GetCase(uint32 p_PosX, uint32 p_PosY) const
{
    if ((uint32)((p_PosY / TILE_SIZE) * m_SizeX) + (p_PosX / TILE_SIZE) >= m_ListCase.size())
        return nullptr;

    return m_ListCase[(uint32)((p_PosY / TILE_SIZE) * m_SizeX) + (p_PosX / TILE_SIZE)];
}

uint16 Map::GetSquareID(uint16 p_X, uint16 p_Y) const
{
    uint16 l_DrawingSquareID = 0;

    /// Total square width
    uint16 l_TotalSquareWidth = (uint16)ceil((float)m_SizeX / GRID_SIZE);

    uint16 l_XSquare = (uint16)ceil((p_X / TILE_SIZE) / GRID_SIZE);
    uint16 l_YSquare = (uint16)ceil((p_Y / TILE_SIZE) / GRID_SIZE);

    return (l_TotalSquareWidth * l_YSquare) + l_XSquare;
}

Unit* Map::GetUnit(TypeUnit p_TypeID, uint16 p_UnitID)
{
    if (m_ListUnitZone[p_TypeID].find(p_UnitID) != m_ListUnitZone[p_TypeID].end())
        return m_ListUnitZone[p_TypeID][p_UnitID];
    return nullptr;
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

            l_Unit->Update(p_Diff);
            if (l_Unit->GetMapID() != m_ID || l_Unit->GetInstanceID() != m_InstanceID)
                m_UnitInterMapActionQueue[eInterMapAction::SwitchMap].push(l_Unit);
            else if (l_Unit->GetSquareID() != GetSquareID(l_Unit->GetPosX(), l_Unit->GetPosY()))
                ChangeSquare(l_Unit);
        }
    }
    /*for (std::vector<DynamicObject*>::iterator l_It = m_ListDynamicObjects.begin(); l_It != m_ListDynamicObjects.end(); ++l_It)
    {
        (*l_It)->Update(p_Diff);
    }*/
}

uint16 Map::GetNbUnitType(TypeUnit p_Type)
{
    if (m_ListUnitZone.find(p_Type) == m_ListUnitZone.end())
        return 0;
    return m_ListUnitZone[p_Type].size();
}

bool Map::IsFinish()
{
    return false;
}

std::map<uint16, Unit*>* Map::GetListUnitType(TypeUnit p_Type)
{
    if (m_ListUnitZone.find(p_Type) == m_ListUnitZone.end())
        return nullptr;
    return &m_ListUnitZone[p_Type];
}

std::queue<Unit*>* Map::GetUnitInterMapAction(eInterMapAction p_ID)
{
    if (m_UnitInterMapActionQueue.find(p_ID) == m_UnitInterMapActionQueue.end())
        return nullptr;
    return &m_UnitInterMapActionQueue[p_ID];
}

void Map::AddUnitInterMapAction(eInterMapAction p_Action, Unit* p_Unit)
{
    m_UnitInterMapActionQueue[p_Action].push(p_Unit);
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

            l_Session->SendUnitCreate(p_Unit->GetType(), p_Unit->GetID(), p_Unit->GetName(), p_Unit->GetLevel(), p_Unit->GetResourceNb(eResourceType::Health), p_Unit->GetResourceNb(eResourceType::Mana), p_Unit->GetResourceNb(eResourceType::Alignment), p_Unit->GetSkinID(), p_Unit->GetSizeX(), p_Unit->GetSizeY(),  p_Unit->GetSpeedUint8(), p_Unit->GetMapID(), p_Unit->GetPosition(), p_Unit->GetOrientation(), p_Unit->IsInMovement(), p_Unit->GetMovementHandler()->IsInAttack(), p_Unit->IsBlocking(), p_Unit->IsInGroupWith(l_Player));
        }
    }
}

std::vector<Unit*> Map::GetUnitsInCase(uint32 p_PosX, uint32 p_PosY)
{
    uint16 l_SquareID = GetSquareID(p_PosX, p_PosY);
    uint16 l_CaseNb = (uint16)((p_PosY / TILE_SIZE) * m_SizeX) + (p_PosX / TILE_SIZE);
    Square* l_Square = GetSquare(l_SquareID);
    std::vector<Unit*> l_List;

    std::map<TypeUnit, std::map<uint16, Unit*>>* l_SquareList = l_Square->GetList();
    for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_SquareMap : *l_SquareList)
    {
        for (std::pair<uint16, Unit*> l_SquareList : l_SquareMap.second)
        {
            Unit* l_Unit = l_SquareList.second;
            if (l_Unit->GetType() >= TypeUnit::GAMEOBJECT)
                continue;
            if (l_CaseNb == (uint16)((l_Unit->GetPosY() / TILE_SIZE) * m_SizeX) + (l_Unit->GetPosX() / TILE_SIZE))
                l_List.push_back(l_Unit);
        }
    }
    return l_List;
}

Unit* Map::GetCloserUnit(Unit* p_Unit, float p_Range /*= 2.0f*/, bool p_OnlyInLife /*= flase*/, bool p_InFront /*= true*/, bool p_Attackable /*= false*/)
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

                if (l_Unit == nullptr || !l_Unit->IsInWorld() || l_Unit->IsInEvade())
                    continue;

                if (l_Unit == p_Unit)
                    continue;

                if (p_OnlyInLife && l_Unit->IsDeath())
                    continue;

                if (p_Attackable && !p_Unit->CanAttack(l_Unit))
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

std::vector<Unit*> Map::GetUnitsInRadius(WorldObject* p_WorldObject, float p_RangeMin, float p_RangeMax, bool p_OnlyInLife, float p_Angle)
{
    /// TODO : QuadTree
    std::vector<Square*> l_Grid = GetSquareSet(GetSquareID(p_WorldObject->GetPosX(), p_WorldObject->GetPosY()));
    std::vector<Unit*> l_Result;

    for (uint8 i = 0; i < l_Grid.size(); ++i)
    {
        Square* l_Square = l_Grid[i];

        std::map<TypeUnit, std::map<uint16, Unit*>>* l_SquareList = l_Square->GetList();

        for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_SquareMap : *l_SquareList)
        {
            for (std::pair<uint16, Unit*> l_SquareList : l_SquareMap.second)
            {
                Unit* l_Unit = l_SquareList.second;

                if (l_Unit == nullptr || !l_Unit->IsInWorld() || l_Unit->IsInEvade())
                    continue;

                if (p_OnlyInLife && l_Unit->IsDeath())
                    continue;

                float l_Dist = InYard(p_WorldObject->GetDistance(l_Unit));

                if (l_Dist > p_RangeMax || l_Dist < p_RangeMin)
                    continue;

                /// TODO : cheack angle
                /*if (p_InFront && !p_Unit->IsInFront(l_Unit))
                continue;*/

                l_Result.push_back(l_Unit);
            }
        }
    }
    return l_Result;
}

std::vector<Unit*> Map::GetUnitsInRadius(Unit* p_Unit, float p_RangeMin, float p_RangeMax, bool p_OnlyInLife /*= false*/, bool p_Attackable /*= false*/, float p_Angle /*= 360.0f*/)
{
    /// TODO : QuadTree
    std::vector<Square*> l_Grid = GetSquareSet(p_Unit->GetSquareID());
    std::vector<Unit*> l_Result;

    for (uint8 i = 0; i < l_Grid.size(); ++i)
    {
        Square* l_Square = l_Grid[i];

        std::map<TypeUnit, std::map<uint16, Unit*>>* l_SquareList = l_Square->GetList();

        for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_SquareMap : *l_SquareList)
        {
            for (std::pair<uint16, Unit*> l_SquareList : l_SquareMap.second)
            {
                Unit* l_Unit = l_SquareList.second;

                if (l_Unit == nullptr || !l_Unit->IsInWorld() || l_Unit->IsInEvade())
                    continue;

                if (l_Unit == p_Unit)
                    continue;

                if (p_OnlyInLife && l_Unit->IsDeath())
                    continue;

                if (p_Attackable && !p_Unit->CanAttack(l_Unit))
                    continue;

                float l_Dist = InYard(p_Unit->GetDistance(l_Unit));

                if (l_Dist > p_RangeMax || l_Dist < p_RangeMin)
                    continue;

                /// TODO : cheack angle
                /*if (p_InFront && !p_Unit->IsInFront(l_Unit))
                    continue;*/

                l_Result.push_back(l_Unit);
            }
        }
    }
    return l_Result;
}

void Map::AddUnit(Unit* p_Unit)
{
    p_Unit->SetMap(this);
    p_Unit->SetInWorld(true);
    m_ListUnitZone[p_Unit->GetType()][p_Unit->GetID()] = p_Unit;

    if (p_Unit->GetPosX() / TILE_SIZE > GetSizeX() || p_Unit->GetPosY() / TILE_SIZE > GetSizeX())
    {
        p_Unit->SetPosX(10);
        p_Unit->SetPosY(10);
    }
    /// Add to square
    uint16 l_SquareId = GetSquareID(p_Unit->GetPosX(), p_Unit->GetPosY());
    AddToSquare(p_Unit, l_SquareId);
    UpdateForPlayersInNewSquare(p_Unit, true);
    if (p_Unit->GetType() == TypeUnit::PLAYER)
        p_Unit->ToPlayer()->UpdateNewSquares(0, p_Unit->GetSquareID(), true);

    GetCase(((p_Unit->GetPositionCentered().m_Y / TILE_SIZE) * (uint32)GetSizeX()) + (p_Unit->GetPositionCentered().m_X / TILE_SIZE))->UnitEnterInCase(p_Unit, nullptr);
}

void Map::RemoveUnit(Unit* p_Unit)
{
    m_ListUnitZone[p_Unit->GetType()].erase(p_Unit->GetID());

    Case* l_Case = GetCase(((p_Unit->GetPositionCentered().m_Y / TILE_SIZE) * (uint32)GetSizeX()) + (p_Unit->GetPositionCentered().m_X / TILE_SIZE));
    if (l_Case != nullptr)
        l_Case->UnitOutOfCase(p_Unit, nullptr);
    //GetCase(((p_Unit->GetPosY() / TILE_SIZE) * (uint32)GetSizeX()) + (p_Unit->GetPosX() / TILE_SIZE))->UnitOutOfCase(p_Unit, nullptr);

    /// Remove from square
    RemoveFromSquare(p_Unit);
    if (m_GroupManager != nullptr)
        m_GroupManager->RemoveUnitFromAllGroup(p_Unit);

    PacketUnitRemove l_Packet;
    l_Packet.BuildPacket(p_Unit->GetType(), p_Unit->GetID());
    SendToSet(l_Packet.m_Packet, p_Unit);
    p_Unit->SetInWorld(false);
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
	for (uint32 i = 0; i < (m_SizeX * m_SizeY); ++i)
	{
		t_Case l_FluxCase;
		fread(&l_FluxCase, sizeof(l_FluxCase), 1, l_File);
		Case* l_Case = new Case(i, i % m_SizeX, i / m_SizeX);
        l_Case->SetMapID(m_ID);

		bool l_Block = l_FluxCase.l_Block;
		if (l_Block)
			l_Case->SetBlock(true);
		m_ListCase.push_back(l_Case);
    }

	return true;
}

void Map::AddCase(Case* p_Case)
{
    m_ListCase.push_back(p_Case);
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

void Map::AddZone(Zone* p_Zone)
{
    m_ListZone[p_Zone->m_ID] = p_Zone;

    uint16 l_XBeginLine = p_Zone->m_CaseBegin % GetSizeX();
    uint16 l_XEndLine = p_Zone->m_CaseEnd % GetSizeX();

    if (l_XEndLine < l_XBeginLine)
        return;

    for (uint32 i = p_Zone->m_CaseBegin; i <= p_Zone->m_CaseEnd;)
    {
        Case* l_Case = GetCase(i);

        if (l_Case == nullptr)
            continue;

        l_Case->AddZone(p_Zone);

        if ((i + 1) % GetSizeX() > l_XEndLine)
            i += (GetSizeX() - (l_XEndLine - l_XBeginLine));
        else
            i++;
    }
}

void Map::EnableZone(uint16 p_Id,  bool p_Enable)
{
    if (m_ListZone.find(p_Id) == m_ListZone.end())
        return;

    m_ListZone[p_Id]->m_enabled = p_Enable;
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

GroupManager* Map::GetGroupManager()
{
    return m_GroupManager;
}