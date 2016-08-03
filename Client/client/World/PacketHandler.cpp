#include "PacketHandler.hpp"
#include "../Entities/Player.hpp"
#include "../Global.hpp"


PacketHandler::PacketHandler(MapManager *p_MapManager) :
	m_MapManager(p_MapManager)
{
    p_HasMinimalRequiered = false;
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
	m_PacketHandleMap[1] = &PacketHandler::HandleConnexion;
    m_PacketHandleMap[10] = &PacketHandler::HandleCreateMainPlayer;
    m_PacketHandleMap[11] = &PacketHandler::HandleCreateUnit;
    m_PacketHandleMap[12] = &PacketHandler::HandleRemoveUnit;
    m_PacketHandleMap[20] = &PacketHandler::HandleUnitGoDirection;
    m_PacketHandleMap[21] = &PacketHandler::HandleStopMovement;
    m_PacketHandleMap[22] = &PacketHandler::HandleUpdatePosition;
}

void PacketHandler::HandleRemoveUnit(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
            return;

        l_Map->RemoveUnit(l_Unit);
        delete l_Unit;
    }
}

void PacketHandler::HandleStopMovement(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_ID;
    uint32 l_PosX;
    uint32 l_PosY;
    uint8 l_Orientation;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    printf("Stop movement for Type :%d, ID:%d, posX:%d, posY:%d, orientation:%d\n", l_TypeID, l_ID, l_PosX, l_PosY, l_Orientation);
    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_ID); ///< Ask for unknow unit to server
            return;
        }

        l_Unit->GetMovementHandler()->StopMovement();
        l_Unit->SetPosX(l_PosX);
        l_Unit->SetPosY(l_PosY);
    }
}

void PacketHandler::HandleUpdatePosition(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint16 l_UnitID;
    uint32 l_PosX;
    uint32 l_PosY;

    p_Packet >> l_TypeID;
    p_Packet >> l_UnitID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_UnitID);

        if (l_Unit == nullptr)
        {
            g_Socket->SendUnitUnknow(l_TypeID, l_UnitID); ///< Ask for unknow unit to server
            return;
        }
        l_Unit->SetPosX(l_PosX);
        l_Unit->SetPosY(l_PosY);
    }
}

void PacketHandler::HandleUnitGoDirection(WorldPacket &p_Packet)
{
    uint8 l_Type;
    uint16 l_UnitID;
    uint8 l_Direction;

    p_Packet >> l_Type;
    p_Packet >> l_UnitID;
    p_Packet >> l_Direction;

    if (Map* l_Map = m_MapManager->GetActualMap())
        l_Map->MoveUnitToDirection((TypeUnit)l_Type, l_UnitID, l_Direction);
}

void PacketHandler::HandleConnexion(WorldPacket &p_Packet)
{
	uint8 l_Status;

	p_Packet >> l_Status;
	printf("Auth Status: %d\n", l_Status);

    switch (l_Status)
    {
    case 0:
        printf("Auth Failed\n");
        break;
    case 1:
        printf("Auth Success\n");
        break;
    case 2:
        printf("Already connected\n");
        break;
    }
}

void PacketHandler::HandleCreateMainPlayer(WorldPacket &p_Packet)
{
    uint32 l_ID;
    std::string l_Name;
    uint8 l_Level;
    uint8 l_SkinID;
    uint16 l_MapID;
    uint32 l_PosX;
    uint32 l_PosY;
    uint8 l_Orientation;

    p_Packet >> l_ID;
    p_Packet >> l_Name;
    p_Packet >> l_Level;
    p_Packet >> l_SkinID;
    p_Packet >> l_MapID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;

    printf("Create me: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_PosX, l_PosY);
    if (!m_MapManager->LoadMap(l_MapID))
        return;

    g_Player = new Player(l_ID, l_Name, l_Level, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);
    m_MapManager->SetPosX(l_PosX);
    m_MapManager->SetPosY(l_PosY);

    if (Map* l_ActualMap = m_MapManager->GetActualMap())
    {
        g_Player->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(g_Player);
        p_HasMinimalRequiered = true;
    }
    else
        delete g_Player;
}

void PacketHandler::HandleCreateUnit(WorldPacket &p_Packet)
{
    uint8 l_TypeID;
    uint32 l_ID;
    std::string l_Name;
    uint8 l_Level;
    uint8 l_SkinID;
    uint16 l_MapID;
    uint32 l_PosX;
    uint32 l_PosY;
    uint8 l_Orientation;
    bool l_IsInMovement;

    p_Packet >> l_TypeID;
    p_Packet >> l_ID;
    p_Packet >> l_Name;
    p_Packet >> l_Level;
    p_Packet >> l_SkinID;
    p_Packet >> l_MapID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;
    p_Packet >> l_Orientation;
    p_Packet >> l_IsInMovement;

    printf("Create new Player: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_PosX, l_PosY);

    Unit* l_NewUnit = nullptr;

    Map* l_ActualMap = m_MapManager->GetActualMap();

    if (l_ActualMap == nullptr)
        return;

    if (l_ActualMap->GetID() == l_MapID && l_ActualMap->GetUnit((TypeUnit)l_TypeID, l_ID) == nullptr)
    {
        if (l_TypeID == (uint8)TypeUnit::PLAYER)
            l_NewUnit = new Player(l_ID, l_Name, l_Level, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);

        l_NewUnit->SetMap(l_ActualMap);
        l_ActualMap->AddUnit(l_NewUnit);
        if (l_IsInMovement)
            l_NewUnit->StartMovement();
    }
}

void PacketHandler::OperatePacket(WorldPacket &p_Packet)
{
	uint8 l_PacketID;
	p_Packet >> l_PacketID;
	printf("Receive Packet %d\n", l_PacketID);
	m_Func l_Fun = m_PacketHandleMap[l_PacketID];
	if (l_Fun != nullptr)
		(this->*(l_Fun))(p_Packet);
	else
		printf("Packet %d Unknow\n", l_PacketID);
}

bool PacketHandler::HasMinimalRequiered() const
{
    return p_HasMinimalRequiered;
}