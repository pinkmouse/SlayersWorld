#include "World.hpp"
#include "SFML/Network/Packet.hpp"
#include <vector>

World::World()
{
	l_Socket = new Socket(
);
	l_Graphics = new Graphics();
}

World::~World()
{
}

void World::Run()
{
	l_Graphics->CreateWindow(500, 500);
	printf("Passe");
	sf::Packet packet;
	packet << "coucou" << 12;
	if (!l_Socket->Connection())
		printf("Not Connected");
	if (l_Socket->send(packet) != sf::Socket::Done)
	{
		printf("Error Sent");
	}
	while (1)
	{
		;
	}
}