#include "../stdafx.h"
#include "World.hpp"
#include "WorldPacket.hpp"


World::World()
	: m_Thread(&World::NetworkLoop, this),
	m_Run(true)
{
	
}


World::~World()
{
}

void World::Run()
{
	if (!this->NetworkInitialize())
		printf("Network error");
	m_Thread.launch();

	while (m_Run)
	{
		this->UpdatePacketQueue();
	}
}

void World::UpdatePacketQueue()
{
	for (std::vector<std::pair<WorldSocket*, WorldPacket>>::iterator l_It = m_PaquetQueue.begin(); l_It != m_PaquetQueue.end();)
	{

		printf("Lecture Paquet");
		l_It = m_PaquetQueue.erase(l_It);
		printf("Fin lecture");
	}
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
			m_Mutex.lock();
			m_Mutex.unlock();
			if (m_Selector.isReady(m_Listener))
			{
				WorldSocket* l_NewWorldSocket = new WorldSocket();
				if (m_Listener.accept(*l_NewWorldSocket) == sf::Socket::Done)
				{
					printf("New connection\n");
					l_NewWorldSocket->setBlocking(false);
					m_Sessions.push_back(l_NewWorldSocket);
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
				for (std::vector<WorldSocket*>::iterator l_It = m_Sessions.begin(); l_It != m_Sessions.end(); ++l_It)
				{
					WorldSocket* l_Session = (*l_It);
					if (m_Selector.isReady(*l_Session))
					{
						WorldPacket l_Packet;
						sf::Socket::Status l_SocketStatus;
						l_SocketStatus = l_Session->receive(l_Packet);
						printf("Code Errror Socket : %d\n", l_SocketStatus);
						if (l_SocketStatus == sf::Socket::Status::Done) ///< Reception OK
						{
							std::pair<WorldSocket*, WorldPacket> l_PaquetElement;

							l_PaquetElement.first = l_Session;
							l_PaquetElement.second = l_Packet;

							m_Mutex.lock();
							m_PaquetQueue.push_back(l_PaquetElement);
							m_Mutex.unlock();
						}
						else if (l_SocketStatus == sf::Socket::Status::Disconnected) ///< Disconnecetd
						{
							std::vector<WorldSocket*>::iterator l_It = std::find(m_Sessions.begin(), m_Sessions.end(), l_Session);
							
							/*if (l_It != m_Sessions.end())
								m_Sessions.erase(l_It);*/

							m_Selector.remove(*l_Session);
						}
					}
				}
			}
		}
	}
}