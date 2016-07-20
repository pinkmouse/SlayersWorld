#include "Graphics.hpp"

Graphics::Graphics(MapManager* p_MapManager) :
	m_MapManager(p_MapManager),
	m_ThreadDraw(&Graphics::UpdateWindow, this),
    m_Run(true)
{
	m_TileSet = nullptr;
    m_SkinsManager = nullptr;
}


Graphics::~Graphics()
{
}

void Graphics::CreateWindow(uint32 p_X, uint32 p_Y, float p_Zoom)
{
	m_Window.create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
	m_View = m_Window.getDefaultView();
	m_View.zoom(p_Zoom);
	m_Window.setView(m_View);

	m_TileSet = new TileSet();
	m_TileSet->BuildSprites();

    m_SkinsManager = new SkinsManager();
    m_SkinsManager->LoadSkins();
}

void Graphics::Run()
{
	m_ThreadDraw.launch();
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
    Player* l_MainPlayer = m_MapManager->GetMainPlayer();

	std::vector<std::vector<Case*>> l_SquareZone = l_Map->GetSquareZone(l_Map->GetSquareID(m_MapManager->GetPosX() / TILE_SIZE, m_MapManager->GetPosY() / TILE_SIZE));
	printf("Square Acutal = %d\n", l_Map->GetSquareID(m_MapManager->GetPosX() / TILE_SIZE, m_MapManager->GetPosY() / TILE_SIZE));
	if (l_SquareZone.empty())
		return;
    printf("Draw level 1\n");

    /// First two Level
	for (std::vector<std::vector<Case*>>::iterator l_It = l_SquareZone.begin(); l_It != l_SquareZone.end(); ++l_It)
	{
		std::vector<Case*> l_Square = (*l_It);
		for (std::vector<Case*>::iterator l_It2 = l_Square.begin(); l_It2 != l_Square.end(); ++l_It2)
		{
            for (uint8 l_LevelNb = 0; l_LevelNb < 2; ++l_LevelNb)
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX() * TILE_SIZE, (float)(*l_It2)->GetPosY() * TILE_SIZE);
                m_Window.draw(*l_TileSprite);
            }
		}

        /// Draw Entities
        if (l_MainPlayer != nullptr)
        {
            SkinSprite* l_SkinSprite = m_SkinsManager->GetSkinSprite(l_MainPlayer->GetSkinID(), 0);
            l_SkinSprite->setPosition((float)m_MapManager->GetPosX(), (float)m_MapManager->GetPosY());
            m_Window.draw(*l_SkinSprite);
        }
	}
}

void Graphics::UpdateWindow()
{
    while (m_Run)
    {
        m_View.setCenter((float)m_MapManager->GetPosX(), (float)m_MapManager->GetPosY());
        m_Window.setView(m_View);
        Clear();
        DrawMap();
        Display();
    }
}

void Graphics::End()
{
	m_Run = false;
}

void Graphics::Clear()
{
	m_Window.clear();
}

void Graphics::Display()
{
	m_Window.display();
}
