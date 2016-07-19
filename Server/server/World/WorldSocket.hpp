#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "WorldPacket.hpp"
#include "../Define.hpp"

class WorldSocket : public sf::TcpSocket
{
public:
	WorldSocket();
	~WorldSocket();

    void SendPlayerCreate(uint32, uint8, uint16, uint32, uint32);
	void SendAuthResponse(uint8);
    void SendMsg(WorldPacket);
};

