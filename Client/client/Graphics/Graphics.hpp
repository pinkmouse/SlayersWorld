#pragma once

#include <SFML/System/Thread.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../Define.hpp"
#include "Window.hpp"
#include <SFML/Graphics/View.hpp>
#include "../Map/MapManager.hpp"
#include "TileSet.hpp"
#include "SkinsManager.hpp"
#include "../World/Events/Events.hpp"

class Graphics
{
public:
	Graphics(MapManager*, Events*);
	~Graphics();
	void CreateWindow(uint32, uint32, float);
	void UpdateWindow();
	void DrawMap();
	void Display();
	void Clear();
	void CheckEvent();
    void Run();
	void End();
	bool WindowIsOpen() const;

private:
	Window          m_Window;
	sf::View        m_View;
	sf::Thread      m_ThreadDraw;
	MapManager*     m_MapManager;
	TileSet*        m_TileSet;
    SkinsManager*   m_SkinsManager;
    Events*         m_Events;
    bool            m_Run;
};

