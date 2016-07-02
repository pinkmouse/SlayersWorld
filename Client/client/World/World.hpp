#pragma once

#include "../Socket/Socket.hpp"
#include "../Graphics/Graphics.hpp"

class World
{
public:
	World();
	~World();
	void Run();
	void End();
	void Initialize(char**);
	void Login(char**);
	bool InitializeConnection();
	bool InitializeWindow();

private:
	Socket* l_Socket;
	Graphics* l_Graphics;
	bool m_Run;

	Window m_Window;
};

