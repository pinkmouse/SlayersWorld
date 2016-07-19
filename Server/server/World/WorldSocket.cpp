#include "../stdafx.h"
#include "WorldSocket.hpp"

WorldSocket::WorldSocket()
{
}


WorldSocket::~WorldSocket()
{
}

void WorldSocket::SendAuthResponse(uint8 p_Status)
{
	WorldPacket l_Packet;
	uint8 l_ID = 1;

	l_Packet << l_ID << p_Status;
	send(l_Packet);
	printf("Send Status Auth %d\n", p_Status);
}

void WorldSocket::SendPlayerCreate(uint32 p_ID, uint8 p_SkinID, uint16 p_MapID, uint32 p_PosX, uint32 p_PosY)
{
    WorldPacket l_Packet;
    uint8 l_ID = 10;

    l_Packet << l_ID << p_ID << p_SkinID << p_MapID << p_PosX << p_PosY;
    send(l_Packet);
    printf("Send create\n");
}

void WorldSocket::SendMsg(WorldPacket p_Packet)
{
    send(p_Packet);
}