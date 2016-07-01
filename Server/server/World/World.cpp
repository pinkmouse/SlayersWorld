#include "../stdafx.h"
#include "World.hpp"
#include "WorldPacket.hpp"


World::World()
    : m_Thread(&World::NetworkLoop, this),
    m_Run(true),
    m_PacketHandler(new PacketHandler)
{
	
}


World::~World()
{
}

void World::Run()
{
	if (!this->NetworkInitialize())
		printf("Network error");
    
    m_PacketHandler->LoadPacketHandlerMap();
    printf("Load Packet Handler...\n");
	m_Thread.launch();

	while (m_Run)
	{
		this->UpdatePacketQueue();
	}
}

void World::UpdatePacketQueue()
{
    m_MutexPacketQueue.lock();

	for (std::vector<std::pair<WorldSocket*, WorldPacket>>::iterator l_It = m_PaquetQueue.begin(); l_It != m_PaquetQueue.end();)
	{
        m_PacketHandler->OperatePacket((*l_It).second, (*l_It).first);
		l_It = m_PaquetQueue.erase(l_It);
	}

    m_MutexPacketQueue.unlock();
}

bool World::NetworkInitialize()
{
	int l_Port = 1234;
	printf("Port: %d -> ", l_Port);
	sf::Socket::Status l_ErrorListen = m_Listener.listen(l_Port);
	switch (l_ErrorListen)
	{
	case sf::Socket::Done:
		printf("OK\n");
		break;
	case sf::Socket::NotReady:
		printf("Not Ready\n");
		return false;
		break;
	case sf::Socket::Partial:
		printf("Partial\n");
		return false;
		break;
	case sf::Socket::Disconnected:
		printf("Disconnected\n");
		return false;
		break;
	case sf::Socket::Error:
		printf("Error\n");
		return false;
		break;
	default:
		break;
	}
	m_Selector.add(m_Listener);
	return true;
}

void World::NetworkLoop()
{
	bool l_Continue = true;

	while (l_Continue)
	{
		if (m_Selector.wait())
		{
			if (m_Selector.isReady(m_Listener))
			{
				WorldSocket* l_NewWorldSocket = new WorldSocket();
				if (m_Listener.accept(*l_NewWorldSocket) == sf::Socket::Done)
				{
					printf("New connection\n");
					l_NewWorldSocket->setBlocking(false);

                    m_MutexSessions.lock();
					m_Sessions.push_back(l_NewWorldSocket);
                    m_MutexSessions.unlock();
					
                    m_Selector.add(*l_NewWorldSocket);
				}
				else
				{
					delete l_NewWorldSocket;
				}
			}
			else
			{
				/// Check if receive data
                m_MutexSessions.lock();

                bool l_IncIt = true;;
				for (std::vector<WorldSocket*>::iterator l_It = m_Sessions.begin(); l_It != m_Sessions.end();)
				{
					WorldSocket* l_Session = (*l_It);
					if (m_Selector.isReady(*l_Session))
					{
						WorldPacket l_Packet;
						sf::Socket::Status l_SocketStatus;
						l_SocketStatus = l_Session->receive(l_Packet);
						if (l_SocketStatus == sf::Socket::Status::Done) ///< Reception OK
						{
							std::pair<WorldSocket*, WorldPacket> l_PaquetElement;

							l_PaquetElement.first = l_Session;
							l_PaquetElement.second = l_Packet;

                            m_MutexPacketQueue.lock();
							m_PaquetQueue.push_back(l_PaquetElement);
                            m_MutexPacketQueue.unlock();
						}
                        if (l_SocketStatus == sf::Socket::Status::Disconnected) ///< Disconnecetd
                        {
                            m_Selector.remove(*l_Session);
                            l_It = m_Sessions.erase(l_It);
                            l_IncIt = false;
                            printf("Disco\n");
                        }
					}
                    if (l_IncIt)
                        ++l_It;
				}

                m_MutexSessions.unlock();
			}
		}
	}
}