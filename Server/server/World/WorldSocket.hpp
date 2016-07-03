#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "../Define.hpp"

class WorldSocket : public sf::TcpSocket
{
public:
	WorldSocket();
	~WorldSocket();

	void SendAuthResponse(uint8);
};

