#pragma once

#include "SFML/Graphics/Text.hpp"
#include "../Define.hpp"
#include "Window.hpp"
#include "../Map/MapManager.hpp"

class Graphics
{
public:
	Graphics(MapManager*);
	~Graphics();
	void CreateWindow(uint32, uint32);
	void UpdateWindow();
	void Display();
	void Clear();
	void CheckEvent();
	bool WindowIsOpen() const;

private:
	Window m_Window;
	MapManager* m_MapManager;
};

