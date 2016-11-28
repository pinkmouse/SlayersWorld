// client.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "World/World.hpp"
#include <iostream>

ConfigHandler   *g_Config;
Player          *g_Player;
Socket          *g_Socket;
sf::Font        *g_Font;

int main(int argc, char** argv)
{
	World l_World;

	std::string l_Login;
	std::string l_Password;

    if (argc == 4)
    {
        l_World.SetIp(argv[1]);
        l_Login = argv[2];
        l_Password = argv[3];
    }
	else if (argc == 3)
	{
        l_Login = argv[1];
        l_Password = argv[2];
	}
	else
	{
		std::cout << "User: " << std::flush;
		std::getline(std::cin, l_Login);
		std::cout << "Password: " << std::flush;
		std::getline(std::cin, l_Password);
	}

    if (!l_World.Initialize())
    {
        printf("Initialization failed\n");
        while (1)
            ;
    }

	l_World.Login(l_Login, l_Password);

	l_World.Run();

	return 0;
}