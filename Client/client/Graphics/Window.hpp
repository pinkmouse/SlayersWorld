#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Window : public sf::RenderWindow
{
public:
	Window();
	~Window();

	sf::Vector2f mapCoordsToPixel(const sf::Vector2f& point, const sf::View& view) const;
    sf::Vector2f mapPixelToCoords(const sf::Vector2f& point, const sf::View& view) const;
};