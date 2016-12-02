#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Window : public sf::RenderWindow
{
public:
	Window();
	~Window();

	sf::Vector2f mapCoordsToPixelFloat(const sf::Vector2f& point, const sf::View& view) const;
};