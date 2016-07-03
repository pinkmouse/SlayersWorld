#pragma once

#include "../Socket/Socket.hpp"
#include "../Graphics/Graphics.hpp"
#include "PacketHandler.hpp"

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
	Socket* m_Socket;
	Graphics* m_Graphics;
	bool m_Run;

	PacketHandler* m_PacketHandler;
	Window m_Window;
};

