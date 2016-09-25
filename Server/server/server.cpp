#include "stdafx.h"
#include "Map/Map.hpp"
#include "World/World.hpp"
#include <Nazara/Network/Network.hpp>
#include <NDK/Application.hpp>

ConfigHandler *g_Config;

int main(int argc, char* argv[])
{
	Ndk::Application app(argc, argv);
	Nz::Initializer<Nz::Network> network;

	printf("Starting Slayers World...\n");
	World* l_World = new World();
	l_World->Run();
	return 0;
}