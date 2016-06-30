#include "Graphics.hpp"



Graphics::Graphics()
{
	m_Window = new Window();
}


Graphics::~Graphics()
{
}

void Graphics::CreateWindow(uint32 p_X, uint32 p_Y)
{
	m_Window->create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
}
