#include "Window.hpp"



Window::Window()
{
}


Window::~Window()
{
}

sf::Vector2f Window::mapCoordsToPixelFloat(const sf::Vector2f& p_Point, const sf::View& p_View) const
{
	sf::Vector2f l_Normalized = p_View.getTransform().transformPoint(p_Point);

	// Then convert to viewport coordinates
	sf::Vector2f l_Pixel;
	sf::IntRect l_Viewport = getViewport(p_View);
	l_Pixel.x = ((l_Normalized.x + 1.f) / 2.f * l_Viewport.width + l_Viewport.left);
	l_Pixel.y = ((-l_Normalized.y + 1.f) / 2.f * l_Viewport.height + l_Viewport.top);

	return l_Pixel;
}