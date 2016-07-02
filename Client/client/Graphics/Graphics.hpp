#pragma once

#include "../Define.hpp"
#include "Window.hpp"

class Graphics
{
public:
	Graphics();
	~Graphics();
	void CreateWindow(uint32, uint32);
	void Display();
	void CheckEvent();
	bool WindowIsOpen() const;

private:
	Window m_Window;
};

