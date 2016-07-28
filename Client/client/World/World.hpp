#pragma once

#include "../Graphics/Graphics.hpp"
#include "../Map/MapManager.hpp"
#include "Events/Events.hpp"
#include "PacketHandler.hpp"
#include "ClockHandler.hpp"

class World
{
public:
	World();
	~World();
	void Run();
	bool UpdateSocket();

	void End();
	void Initialize(char**);
	void Login(char**);
	bool InitializeConnection();
	bool InitializeWindow();

private:
	Graphics* m_Graphics;
	MapManager* m_MapManager;
    Events* m_Events;
	bool m_Run;
    ClockHandler m_Clock;

	PacketHandler* m_PacketHandler;
	Window m_Window;
};
