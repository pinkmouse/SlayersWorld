#include "Window.hpp"



Window::Window()
{
}


Window::~Window()
{
}

sf::Vector2f Window::mapCoordsToPixel(const sf::Vector2f& p_Point, const sf::View& p_View) const
{
	sf::Vector2f l_Normalized = p_View.getTransform().transformPoint(p_Point);

	// Then convert to viewport coordinates
	sf::Vector2f l_Pixel;
	sf::IntRect l_Viewport = getViewport(p_View);
	l_Pixel.x = ((l_Normalized.x + 1.f) / 2.f * l_Viewport.width + l_Viewport.left);
	l_Pixel.y = ((-l_Normalized.y + 1.f) / 2.f * l_Viewport.height + l_Viewport.top);

	return l_Pixel;
}

sf::Vector2f Window::mapPixelToCoords(const sf::Vector2f& p_Point, const sf::View& p_View) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    sf::Vector2f l_Normalized;
    sf::IntRect l_Viewport = getViewport(p_View);
    l_Normalized.x = -1.f + 2.f * (p_Point.x - l_Viewport.left) / l_Viewport.width;
    l_Normalized.y = 1.f - 2.f * (p_Point.y - l_Viewport.top) / l_Viewport.height;

    // Then transform by the inverse of the view matrix
    return p_View.getInverseTransform().transformPoint(l_Normalized);
}