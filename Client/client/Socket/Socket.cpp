#include "Socket.hpp"

#include <SFML/Network/IpAddress.hpp>


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