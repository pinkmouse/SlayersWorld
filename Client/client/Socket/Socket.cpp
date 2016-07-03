#include "Socket.hpp"

#include <SFML/Network/IpAddress.hpp>
#include "../World/WorldPacket.hpp"


Socket::Socket()
{
}


Socket::~Socket()
{
}

bool Socket::Connection()
{
	sf::Socket::Status l_Status = this->connect(sf::IpAddress("127.0.0.1"), 1234);

	if (l_Status == sf::Socket::Status::Done)
		return true;

	return false;
}

void Socket::SendAuth(const std::string & l_Login, const std::string & l_Password)
{
	WorldPacket packet;
	uint8 l_ID = 1;

	packet << l_ID << l_Login << l_Password;

	send(packet);
}
