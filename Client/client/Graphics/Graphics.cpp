#include "Graphics.hpp"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::CreateWindow(uint32 p_X, uint32 p_Y)
{
	m_Window.create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
}

void Graphics::CheckEvent()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		// Request for closing the window
		if (event.type == sf::Event::Closed)
		{
			m_Window.close();
		}
	}
}

bool Graphics::WindowIsOpen() const
{
	return m_Window.isOpen();
}

void Graphics::Display()
{
	m_Window.setActive();
	m_Window.display();
}
