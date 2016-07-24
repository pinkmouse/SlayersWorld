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
    m_PacketHandleMap[10] = &PacketHandler::HandleCreatePlayer;
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

    printf("Create me: %d %s %d %d %d %d\n", l_ID, l_Name.c_str(), l_SkinID, l_MapID, l_PosX, l_PosY);
    if (!m_MapManager->LoadMap(l_MapID))
        return;

    g_Player = new Player(l_ID, l_Name, l_Level, l_SkinID, l_MapID, l_PosX, l_PosY, (Orientation)l_Orientation);
    m_MapManager->SetPosX(l_PosX);
    m_MapManager->SetPosY(l_PosY);
    m_MapManager->GetActualMap()->AddPlayer(g_Player);
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

