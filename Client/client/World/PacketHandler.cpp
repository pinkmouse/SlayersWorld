#include "PacketHandler.hpp"


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
    uint8 l_SkinID;
    uint16 l_MapID;
    uint32 l_PosX;
    uint32 l_PosY;

    p_Packet >> l_ID;
    p_Packet >> l_SkinID;
    p_Packet >> l_MapID;
    p_Packet >> l_PosX;
    p_Packet >> l_PosY;

    printf("Create me: %d %d %d %d %d\n", l_ID, l_SkinID, l_MapID, l_PosX, l_PosY);
    if (!m_MapManager->LoadMap(l_MapID))
        return;

    m_MapManager->SetPosX(l_PosX);
    m_MapManager->SetPosY(l_PosY);
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

