// client.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "World/World.hpp"
#include <iostream>

Player      *g_Player;
Socket      *g_Socket;
sf::Font    *g_Font;

int main(int argc, char** argv)
{
	World l_World;
	l_World.Initialize();

	std::string login;
	std::string password;

	if (argc == 3)
	{
		login = argv[1];
		password = argv[2];
	}
	else
	{
		std::cout << "User: " << std::flush;
		std::getline(std::cin, login);
		std::cout << "Password: " << std::flush;
		std::getline(std::cin, password);
	}

	l_World.Login(login, password);

	l_World.Run();

	return 0;
}