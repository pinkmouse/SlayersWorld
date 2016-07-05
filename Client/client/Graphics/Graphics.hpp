#pragma once

#include <SFML/Graphics/Text.hpp>
#include "../Define.hpp"
#include "Window.hpp"
#include <SFML/Graphics/View.hpp>
#include "../Map/MapManager.hpp"
#include "TileSet.hpp"

class Graphics
{
public:
	Graphics(MapManager*);
	~Graphics();
	void CreateWindow(uint32, uint32);
	void UpdateWindow();
	void DrawMap();
	void Display();
	void Clear();
	void CheckEvent();
	bool WindowIsOpen() const;

private:
	Window m_Window;
	sf::View m_View;
	MapManager* m_MapManager;
	TileSet* m_TileSet;
};

