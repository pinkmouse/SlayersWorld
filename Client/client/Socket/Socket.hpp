#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include "../Define.hpp"

class Socket : public sf::TcpSocket
{
public:
	Socket();
	~Socket();
	bool Connection();
	void SendAuth(const std::string & l_Login, const std::string & l_Password);
};
