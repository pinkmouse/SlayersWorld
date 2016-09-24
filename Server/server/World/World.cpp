#include "../stdafx.h"
#include <Nazara/Core/LockGuard.hpp>
#include "World.hpp"

World::World()
    :
    m_Run(true),
    m_SqlManager(new SqlManager()),
	m_MapManager(new MapManager()),
    m_PacketHandler(new PacketHandler(m_SqlManager, m_MapManager)),
    m_CreatureManager(new CreatureManager())
{
	g_Config = new ConfigHandler();
}


World::~World()
{
}

bool World::Initialize()
{
	printf("Load Config...\n");
	if (!g_Config->Initialize())
	{
		printf("Config error\n");
		return false;
	}
	printf("Load Maps...\n");
	if (!m_MapManager->InitializeMaps())
	{
		printf("Load Maps Error\n");
		return false;
	}
    printf("Connection SQL...\n");

    std::vector<std::string> l_ConfigSQLWorld = g_Config->GetValueList(g_Config->GetValue("worldDB"));
    if (!m_SqlManager->InitializeWorld(l_ConfigSQLWorld[0], l_ConfigSQLWorld[1], l_ConfigSQLWorld[2], l_ConfigSQLWorld[3], l_ConfigSQLWorld[4]))
        printf("Error connection world SQL...\n");

    printf("Initialize CreatureTemplate\n");
    if (!m_SqlManager->InitializeCreatureTemplate(m_CreatureManager))
        printf("Error Initialize CreatureTemplate...\n");

    std::vector<std::string> l_ConfigSQLCharacters = g_Config->GetValueList(g_Config->GetValue("charactersDB"));
    if (!m_SqlManager->InitializeCharacters(l_ConfigSQLCharacters[0], l_ConfigSQLCharacters[1], l_ConfigSQLCharacters[2], l_ConfigSQLCharacters[3], l_ConfigSQLCharacters[4]))
        printf("Error connection character SQL...\n");

	printf("Launch Network...\n");
	if (!this->NetworkInitialize())
	{
		printf("Network error\n");
		return false;
	}

	printf("Load Packet Handler...\n");
	m_PacketHandler->LoadPacketHandlerMap();

	return true;
}

void World::Run()
{
	if (Initialize())
        printf("WorldServer Start\n");
    else
        printf("WorldServer didn't start\n");

	m_Thread = Nz::Thread(&World::NetworkLoop, this);

	while (m_Run)
	{
		UpdatePacketQueue();
        sf::Time l_Diff = m_Clock.restart();
        m_MapManager->Update(l_Diff);
	}
}

void World::UpdatePacketQueue()
{
	Nz::LockGuard lock(m_MutexPacketQueue);

	for (auto l_It = m_PaquetQueue.begin(); l_It != m_PaquetQueue.end();)
	{
        m_PacketHandler->OperatePacket(l_It->second, l_It->first);
		l_It = m_PaquetQueue.erase(l_It);
	}

    for (WorldSocket* l_Socket : m_DisconnectedQueue)
    {
        m_PacketHandler->HandleDisconnected(l_Socket);
        delete l_Socket;
    }

	m_DisconnectedQueue.clear();
}

bool World::NetworkInitialize()
{
	int l_Port = std::stoi(g_Config->GetValue("Port"));
	printf("Port: %d -> ", l_Port);

	Nz::SocketState l_ErrorListen = m_Listener.Listen(Nz::NetProtocol_IPv4, l_Port);
	if (l_ErrorListen != Nz::SocketState_Bound)
	{
		printf("Failed to listen to port %u\n", l_Port);
		return false;
	}

	if (!m_Selector.RegisterSocket(m_Listener))
	{
		printf("Failed to register listener socket\n");
		return false;
	}

	return true;
}

void World::NetworkLoop()
{
	bool l_Continue = true;

	while (l_Continue)
	{
		if (m_Selector.Wait(0U)) //< Indefinite block
		{
			if (m_Selector.IsReady(m_Listener))
			{
				WorldSocket* l_NewWorldSocket = new WorldSocket();
				if (m_Listener.AcceptClient(l_NewWorldSocket))
				{
					printf("New connection\n");
					l_NewWorldSocket->EnableBlocking(false);

                    m_MutexSessions.Lock();
					m_Sessions.push_back(l_NewWorldSocket);
                    m_MutexSessions.Unlock();
					
                    m_Selector.RegisterSocket(*l_NewWorldSocket);
				}
				else
				{
					delete l_NewWorldSocket;
				}
			}
			else
			{
				/// Check if receive data
				Nz::LockGuard sessionLock(m_MutexSessions);

                bool l_IncIt = true;
				for (std::vector<WorldSocket*>::iterator l_It = m_Sessions.begin(); l_It != m_Sessions.end();)
				{
					WorldSocket* l_Session = (*l_It);
					if (m_Selector.IsReady(*l_Session))
					{
						std::pair<WorldSocket*, Nz::NetPacket> l_PaquetElement;
						l_PaquetElement.first = l_Session;

						if (l_Session->ReceivePacket(&l_PaquetElement.second)) ///< Reception OK
						{
							Nz::LockGuard packetQueueLock(m_MutexPacketQueue);

							m_PaquetQueue.emplace_back(std::move(l_PaquetElement));
						}
						else
						{
							if (l_Session->GetState() == Nz::SocketState_NotConnected)
							{
								m_Selector.UnregisterSocket(*l_Session);

								Nz::LockGuard packetQueueLock(m_MutexPacketQueue);

								m_DisconnectedQueue.push_back(l_Session);

								l_It = m_Sessions.erase(l_It);
								l_IncIt = false;
								printf("Disco\n");
							}
						}
					}
                    if (l_IncIt)
                        ++l_It;
                    else
                        l_IncIt = true;
				}
			}
		}
	}
}