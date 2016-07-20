#include "../stdafx.h"
#include "WorldSocket.hpp"

WorldSocket::WorldSocket()
{
    m_Player = nullptr;
}


WorldSocket::~WorldSocket()
{
    delete m_Player;
}

void WorldSocket::SendAuthResponse(uint8 p_Status)
{
	WorldPacket l_Packet;
	uint8 l_ID = 1;

	l_Packet << l_ID << p_Status;
	send(l_Packet);
	printf("Send Status Auth %d\n", p_Status);
}

void WorldSocket::SendPlayerCreate(uint32 p_ID, std::string p_Name, uint8 p_Level, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY, uint8 p_Orientation)
{
    WorldPacket l_Packet;
    uint8 l_ID = 10;

    l_Packet << l_ID << p_ID << p_Name << p_Level << p_SkinID << p_MapID << p_PosX << p_PosY << p_Orientation;
    send(l_Packet);
    printf("Send create\n");
}

Player* WorldSocket::GetPlayer()
{
    return m_Player;
}

void WorldSocket::SetPlayer(Player* p_Player)
{
    m_Player = p_Player;
}

void WorldSocket::SendMsg(WorldPacket p_Packet)
{
    send(p_Packet);
}