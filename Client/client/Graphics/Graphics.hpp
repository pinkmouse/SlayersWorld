#pragma once

#include "SFML/Graphics/Text.hpp"
#include "../Define.hpp"
#include "Window.hpp"

class Graphics
{
public:
	Graphics();
	~Graphics();
	void CreateWindow(uint32, uint32);
	void UpdateWindow();
	void Display();
	void Clear();
	void CheckEvent();
	bool WindowIsOpen() const;

private:
	Window m_Window;
};

