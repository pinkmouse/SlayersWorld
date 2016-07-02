#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Window : public sf::RenderWindow
{
public:
	Window();
	~Window();
};