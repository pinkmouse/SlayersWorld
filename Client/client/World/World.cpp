#include "World.hpp"
#include "WorldPacket.hpp"
#include <vector>

World::World()
{
	l_Socket = new Socket();
	l_Graphics = new Graphics();
	m_Run = true;
}

World::~World()
{
}

bool World::InitializeConnection()
{
	if (!l_Socket->Connection())
	{
		printf("Error Connection...");
		return false;
	}
	return true;
}

bool World::InitializeWindow()
{
	l_Graphics->CreateWindow(500, 500);
	return true;
}

void World::Initialize(char** p_Argv)
{

	InitializeWindow();

	if (!InitializeConnection())
		return;

	Login(p_Argv);
	Run();
}

void World::Login(char** p_Argv)
{
	WorldPacket packet;
	uint8 l_ID = 1;

	std::string l_Login(p_Argv[1]);
	std::string l_Password(p_Argv[2]);

	packet << l_ID << l_Login << l_Password;

	l_Socket->send(packet);
}

void World::Run()
{
	while (m_Run)
	{
		l_Graphics->CheckEvent();
		if (!l_Graphics->WindowIsOpen())
			End();
		l_Graphics->Display();
	}
}

void World::End()
{
	m_Run = false;
	l_Socket->disconnect();
}