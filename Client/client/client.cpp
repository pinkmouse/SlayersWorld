// client.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "World/World.hpp"

Player *g_Player;
Socket *g_Socket;

int main(int argc, char** argv)
{
	World* l_World = new World();
	if (argc == 3)
		l_World->Initialize(argv);

    return 0;
}