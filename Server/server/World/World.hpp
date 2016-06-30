#pragma once

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <vector>
#include "WorldSocket.hpp"
#include "WorldPacket.hpp"

class World
{
public:
	World();
	~World();
	void Run();

private:
	bool m_Run;

	/// NETWORK
	void NetworkLoop();
	bool NetworkInitialize();

	void UpdatePacketQueue();

	sf::Thread m_Thread;
	sf::Mutex m_Mutex;

	sf::SocketSelector m_Selector;
	sf::TcpListener m_Listener;
	std::vector<WorldSocket*> m_Sessions;

	std::vector<std::pair<WorldSocket*, WorldPacket>> m_PaquetQueue;
};

