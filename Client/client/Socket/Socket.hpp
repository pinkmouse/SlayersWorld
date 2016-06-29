#pragma once

#include <SFML/Network/TcpSocket.hpp>

class Socket : public sf::TcpSocket
{
public:
	Socket();
	~Socket();
};
