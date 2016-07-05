#include "Graphics.hpp"



Graphics::Graphics(MapManager* p_MapManager) :
	m_MapManager(p_MapManager)
{
	m_TileSet = nullptr;
}


Graphics::~Graphics()
{
}

void Graphics::CreateWindow(uint32 p_X, uint32 p_Y)
{
	m_Window.create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
	m_View = m_Window.getDefaultView();
	m_Window.setView(m_View);
	m_TileSet = new TileSet();
	m_TileSet->FillSpriteList();
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

void Graphics::DrawMap()
{
	if (m_TileSet == nullptr || m_MapManager->GetActualMap() == nullptr)
		return;

	/// First Level
	int16 l_TileID = m_MapManager->GetActualMap()->GetCase(0)->GetTile(0);
	if (l_TileID < 0)
		return;

	if (l_TileID >= m_TileSet->GetTileListSize())
	{
		printf("PASSE %d, %d", l_TileID, m_TileSet->GetTileListSize());
		return;
	}

	TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
	l_TileSprite->setPosition(0, 0);
	m_Window.draw(*l_TileSprite);
}

void Graphics::UpdateWindow()
{
	Clear();
	DrawMap();
	/*m_Window.draw(someSprite);
	sf::Text text;
	sf::Font font;
	font.loadFromFile("font/arial.ttf");
	text.setFont(font);B
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
