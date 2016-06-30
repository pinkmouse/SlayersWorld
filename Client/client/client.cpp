// client.cpp : définit le point d'entrée pour l'application console.
//
#include "stdafx.h"
#include "World/World.hpp"

int main()
{
	World* l_World = new World();
	l_World->Run();

    return 0;
}