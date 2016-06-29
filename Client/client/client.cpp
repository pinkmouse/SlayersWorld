// client.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "Socket/Socket.hpp"
#include <SFML/Network/IpAddress.hpp>

int main()
{
	Socket l_Socket;
	l_Socket.connect(sf::IpAddress("127.0.0.1"), 1234);
    return 0;
}

