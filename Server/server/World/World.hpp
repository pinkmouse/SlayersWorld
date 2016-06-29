#pragma once

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <list>
#include "WorldSocket.hpp"

class World
{
public:
	World();
	~World();
	void Run();

private:
	void NetworkLoop();
	bool NetworkInitialize();

	sf::Thread m_Thread;
	sf::Mutex m_Mutex;

	sf::SocketSelector m_Selector;
	sf::TcpListener m_Listener;
	std::list<WorldSocket*> m_Sessions;
};

