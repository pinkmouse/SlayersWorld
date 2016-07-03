#include "../stdafx.h"
#include "WorldSocket.hpp"
#include "WorldPacket.hpp"


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
