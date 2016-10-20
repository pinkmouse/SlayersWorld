#pragma once

#include "../Graphics/Graphics.hpp"
#include "../Map/MapManager.hpp"
#include "../Graphics/Interface/InterfaceManager.hpp"
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
	bool Initialize();
	void Login(const std::string&, const std::string&);
	bool InitializeConnection();
	bool InitializeWindow();
    void SetIp(const std::string &);

private:
	Graphics* m_Graphics;
	MapManager* m_MapManager;
    std::string m_Ip;
    Events* m_Events;
	bool m_Run;
    ClockHandler m_Clock;

	PacketHandler* m_PacketHandler;
    InterfaceManager* m_InterfaceManager;
	Window m_Window;
};

