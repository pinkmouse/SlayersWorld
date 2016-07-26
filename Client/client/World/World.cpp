#include "World.hpp"
#include "../Global.hpp"
#include "WorldPacket.hpp"
#include <vector>

World::World()
{
	g_Socket = new Socket();
    m_Events = new Events();
	m_MapManager = new MapManager(m_Events);
	m_Graphics = new Graphics(m_MapManager, m_Events);
	m_PacketHandler = new PacketHandler(m_MapManager);
	m_Run = true;
}

World::~World()
{
}

bool World::InitializeConnection()
{
	if (!g_Socket->Connection())
	{
		printf("Error Connection...\n");
		return false;
	}
    g_Socket->setBlocking(false);
	return true;
}

bool World::InitializeWindow()
{
	m_Graphics->CreateWindow(X_WINDOW, Y_WINDOW, 0.5f);
	return true;
}

void World::Initialize(char** p_Argv)
{
	InitializeWindow();
    m_Graphics->Run();
    while (!InitializeConnection()) ///< While not connected, wait for connection
        ;

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

    g_Socket->SendAuth(l_Login, l_Password);
}

void World::Run()
{
	while (m_Run)
	{
		UpdateSocket();
        sf::Time l_Diff = m_Clock.restart();
        m_MapManager->Update(l_Diff);
		m_Graphics->CheckEvent();
		if (!m_Graphics->WindowIsOpen())
			End();
	}
}

bool World::UpdateSocket()
{
	WorldPacket l_Packet;
	sf::Socket::Status l_SocketStatus;
	l_SocketStatus = g_Socket->receive(l_Packet);
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
	m_Graphics->End();
    g_Socket->disconnect();
}