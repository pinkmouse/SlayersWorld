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
	m_TileSet->BuildSprites();
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
	if (!m_Window.isOpen() || !m_MapManager->HasMap())
		return;

	Map* l_Map = m_MapManager->GetActualMap();
	std::vector<Case*> l_Square = l_Map->GetSquare(l_Map->GetSquareID(0, 0));
	if (l_Square.empty())
		return;

	/// First Level
	for (std::vector<Case*>::iterator l_It = l_Square.begin(); l_It != l_Square.end(); ++l_It)
	{
		int16 l_TileID = (*l_It)->GetTile(0);
		if (l_TileID < 0)
			continue;

		TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
		l_TileSprite->setPosition((float)(*l_It)->GetPosX() * TILE_SIZE, (float)(*l_It)->GetPosY() * TILE_SIZE);
		m_Window.draw(*l_TileSprite);
	}
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
