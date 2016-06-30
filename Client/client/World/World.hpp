#pragma once

#include "../Socket/Socket.hpp"
#include "../Graphics/Graphics.hpp"

class World
{
public:
	World();
	~World();
	void Run();
	bool Connection();

private:
	Socket* l_Socket;
	Graphics* l_Graphics;
};

