#include "PacketHandler.hpp"
#include "../Entities/Player.hpp"
#include "../Global.hpp"


PacketHandler::PacketHandler(MapManager *p_MapManager) :
	m_MapManager(p_MapManager)
{
}

PacketHandler::~PacketHandler()
{
}

void PacketHandler::LoadPacketHandlerMap()
{
	m_PacketHandleMap[1] = &PacketHandler::HandleConnexion;
    m_PacketHandleMap[10] = &PacketHandler::HandleCreateMainPlayer;
    m_PacketHandleMap[11] = &PacketHandler::HandleCreatePlayer;
    m_PacketHandleMap[20] = &PacketHandler::HandleUnitGoDirection;
    m_PacketHandleMap[21] = &PacketHandler::HandleStopMovement;
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

    if (Map* l_Map = m_MapManager->GetActualMap())
    {
        Unit* l_Unit = l_Map->GetUnit((TypeUnit)l_TypeID, l_ID);
        if (l_Unit == nullptr)
            return;

        l_Unit->GetMovementHandler()->StopMovement();
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
    if (!l_Status)
        printf("Auth Failed\n");
    else
        printf("Auth Success\n");
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
        l_ActualMap->AddUnit(g_Player);
    }
    else
        delete g_Player;
}

void PacketHandler::HandleCreatePlayer(WorldPacket &p_Packet)
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

    printf("Create new Player: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_PosX, l_PosY);

    Player* l_NewPlayer = new Player(l_ID, l_Name, l_Level, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);
    m_MapManager->SetPosX(l_PosX);
    m_MapManager->SetPosY(l_PosY);
    if (Map* l_ActualMap = m_MapManager->GetActualMap())
    {
        if (l_ActualMap->GetID() != l_MapID)
            delete l_NewPlayer;
        else
            l_ActualMap->AddUnit(l_NewPlayer);
    }
    else
        delete l_NewPlayer;
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
