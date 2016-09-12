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
	m_Graphics->CreateWindow(X_WINDOW, Y_WINDOW, ZOOM_FACTOR);
    if (!m_Graphics->LoadFont())
        return false;
	return true;
}

void World::Initialize()
{
    if (!InitializeWindow())
        return;
    while (!InitializeConnection()) ///< While not connected, wait for connection
        ;

	m_MapManager->InitializeMaps();
	m_PacketHandler->LoadPacketHandlerMap();
}

void World::Login(const std::string& login, const std::string& password)
{
    g_Socket->SendAuth(login, password);
}

void World::Run()
{
	while (m_Run)
	{
        if (!UpdateSocket())
        {
            End();
            continue;
        }
        if (m_PacketHandler->HasMinimalRequiered())
        {
            sf::Time l_Diff = m_Clock.restart();
            m_MapManager->Update(l_Diff);
        }
        m_Graphics->UpdateWindow();
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
    g_Socket->disconnect();
}