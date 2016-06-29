#pragma once

#include <SFML/Network/TcpSocket.hpp>

class WorldSocket : public sf::TcpSocket
{
public:
	WorldSocket();
	~WorldSocket();
};

