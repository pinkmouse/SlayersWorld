#pragma once

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <vector>
#include "WorldSocket.hpp"
#include "PacketHandler.hpp"

class World
{
public:
	World();
	~World();
	void Run();

private:
	bool m_Run;

    /// PACKET
    std::vector<std::pair<WorldSocket*, WorldPacket>> m_PaquetQueue;
    PacketHandler *m_PacketHandler;

	/// NETWORK
	void NetworkLoop();
	bool NetworkInitialize();

	void UpdatePacketQueue();

	sf::Thread m_Thread;
	sf::Mutex m_MutexSessions;
    sf::Mutex m_MutexPacketQueue;

	sf::SocketSelector m_Selector;
	sf::TcpListener m_Listener;
	std::vector<WorldSocket*> m_Sessions;
};

