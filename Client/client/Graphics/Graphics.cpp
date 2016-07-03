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

void Graphics::UpdateWindow()
{
	Clear();
	/*sf::Text text;
	sf::Font font;
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);
	text.setString("Hello world");
	// choix de la taille des caractères
	text.setCharacterSize(24); // exprimée en pixels, pas en points !

							   // choix de la couleur du texte
	text.setColor(sf::Color::Red);
	m_Window.draw(text);*/
	Display();
}

void Graphics::Clear()
{
	m_Window.clear();
}

void Graphics::Display()
{
	m_Window.display();
}
