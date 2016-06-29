#include "../stdafx.h"
#include "World.hpp"
#include "WorldPacket.hpp"


World::World()
	: m_Thread(&World::NetworkLoop, this)
{
}


World::~World()
{
}

void World::Run()
{
	if (!this->NetworkInitialize())
		printf("Network error");
	this->NetworkLoop();
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
					printf("New Co\n");
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
				for (std::list<WorldSocket*>::iterator l_It = m_Sessions.begin(); l_It != m_Sessions.end(); ++l_It)
				{
					WorldSocket& l_Session = **l_It;
					if (m_Selector.isReady(l_Session))
					{
						// The client has sent some data, we can receive it
						WorldPacket l_Packet;
						if (l_Session.receive(l_Packet) == sf::Socket::Done)
						{
							printf("New Msg\n");
						}
					}
				}
			}
		}
	}
}