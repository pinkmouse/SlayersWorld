#include "Map/Map.hpp"
#include "World/World.hpp"
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

ConfigHandler* g_Config;
SqlManager* g_SqlManager;
LevelManager* g_LevelManager;

int main()
{
	printf("Starting Slayers World...\n");
	World* l_World = new World();
	l_World->Run();
	return 0;
}