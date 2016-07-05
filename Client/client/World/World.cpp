#include "World.hpp"
#include "WorldPacket.hpp"
#include <vector>

World::World()
{
	m_Socket = new Socket();
	m_MapManager = new MapManager();
	m_Graphics = new Graphics(m_MapManager);
	m_PacketHandler = new PacketHandler(m_MapManager);
	m_Run = true;
}

World::~World()
{
}

bool World::InitializeConnection()
{
	if (!m_Socket->Connection())
	{
		printf("Error Connection...");
		return false;
	}
	m_Socket->setBlocking(false);
	return true;
}

bool World::InitializeWindow()
{
	m_Graphics->CreateWindow(500, 500);
	return true;
}

void World::Initialize(char** p_Argv)
{
	InitializeWindow();
	InitializeConnection();

	m_MapManager->InitializeMaps();
	m_PacketHandler->LoadPacketHandlerMap();

	Login(p_Argv);
	Run();
}

void World::Login(char** p_Argv)
{
	WorldPacket packet;
	uint8 l_ID = 1;

	std::string l_Login(p_Argv[1]);
	std::string l_Password(p_Argv[2]);

	m_Socket->SendAuth(l_Login, l_Password);
}

void World::Run()
{
	while (m_Run)
	{
		UpdateSocket();
		m_Graphics->CheckEvent();
		if (!m_Graphics->WindowIsOpen())
			End();
		m_Graphics->UpdateWindow();
	}
}

bool World::UpdateSocket()
{
	WorldPacket l_Packet;
	sf::Socket::Status l_SocketStatus;
	l_SocketStatus = m_Socket->receive(l_Packet);
	if (l_SocketStatus == sf::Socket::Status::Done) ///< Reception OK
		m_PacketHandler->OperatePacket(l_Packet);
	if (l_SocketStatus == sf::Socket::Status::Disconnected) ///< Disconnecetd
	{
		printf("Disco\n");
		return false;
	}
	return true;
}

void World::End()
{
	m_Run = false;
	m_Socket->disconnect();
}