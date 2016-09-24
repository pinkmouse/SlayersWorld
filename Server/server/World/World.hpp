#pragma once

#include <Nazara/Core/Mutex.hpp>
#include <Nazara/Core/Thread.hpp>
#include <Nazara/Network/SocketPoller.hpp>
#include <Nazara/Network/TcpServer.hpp>
#include <vector>
#include "WorldSocket.hpp"
#include "PacketHandler.hpp"
#include "SqlManager.hpp"
#include "ClockHandler.hpp"
#include "../Map/MapManager.hpp"
#include "CreatureManager.hpp"

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
    std::vector<std::pair<WorldSocket*, Nz::NetPacket>> m_PaquetQueue;
    PacketHandler* m_PacketHandler;

    /// CREATURE MANAGER
    CreatureManager* m_CreatureManager;

	/// NETWORK
	void NetworkLoop();
	bool NetworkInitialize();
    std::vector<WorldSocket*> m_DisconnectedQueue;

	void UpdatePacketQueue();

	Nz::Thread m_Thread;
	Nz::Mutex m_MutexSessions;
    Nz::Mutex m_MutexPacketQueue;

	Nz::SocketPoller m_Selector;
	Nz::TcpServer m_Listener;
	std::vector<WorldSocket*> m_Sessions;

    ClockHandler m_Clock;
};

