#pragma once

#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <vector>
#include "WorldSocket.hpp"
#include "PacketHandler.hpp"
#include "SqlManager.hpp"
#include "../Map/MapManager.hpp"

class World
{
public:
	World();
	~World();
	bool Initialize();
	void Run();

private:
	bool m_Run;

	/// MAP
	MapManager* m_MapManager;

	/// SQL
	SqlManager* m_SqlManager;

    /// PACKET
    std::vector<std::pair<WorldSocket*, WorldPacket>> m_PaquetQueue;
    PacketHandler* m_PacketHandler;

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

