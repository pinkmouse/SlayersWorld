#pragma once

#include "../Define.hpp"
#include "Window.hpp"

class Graphics
{
public:
	Graphics();
	~Graphics();
	void CreateWindow(uint32, uint32);

private:
	Window* m_Window;
};

