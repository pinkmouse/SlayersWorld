#include "Graphics.hpp"
#include "../Global.hpp"

Graphics::Graphics(MapManager* p_MapManager, Events* p_Events) :
	m_MapManager(p_MapManager),
    m_Events(p_Events)
{
	m_TileSet = nullptr;
    m_SkinsManager = nullptr;
    m_InterfaceManager = nullptr;
    m_Clock = new ClockHandler();
}


Graphics::~Graphics()
{
}

bool Graphics::LoadFont()
{
    std::string l_FontName = "arial.ttf";
    g_Font = new sf::Font();
    if (!g_Font->loadFromFile(FONT_FOLDER + l_FontName))
        return false;
    return true;
}

void Graphics::CreateWindow(uint32 p_X, uint32 p_Y, float p_Zoom)
{
	m_Window.create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
	m_View = m_Window.getDefaultView();
    m_ViewInterface = m_Window.getDefaultView();
	m_View.zoom(p_Zoom);
	m_Window.setView(m_View);
    m_Window.setFramerateLimit(40);

    m_InterfaceManager = new InterfaceManager(m_Events);
    m_InterfaceManager->Initialize();

	m_TileSet = new TileSet();
	m_TileSet->BuildSprites();

    m_SkinsManager = new SkinsManager();
    m_SkinsManager->LoadSkins();
}

void Graphics::CheckEvent()
{
	sf::Event l_Event;
	while (m_Window.pollEvent(l_Event))
	{
        switch(l_Event.type)
        {
            case sf::Event::Closed: ///< Request for closing the window
                m_Window.close();
                break;
            case sf::Event::KeyPressed: ///< Key Press
                m_Events->NewKeyPressed(l_Event.key.code);
                break;
            case sf::Event::KeyReleased: ///< Key Release
                m_Events->KeyRelease(l_Event.key.code);
                break;
            case sf::Event::TextEntered: ///< Text Entered
                m_Events->TextEntered(l_Event.text.unicode);
                break;
            case sf::Event::Resized: ///< Resize Window
                /*
                std::cout << "new width: " << event.size.width << std::endl;
                std::cout << "new height: " << event.size.height << std::endl;
                */
                break;
            default:
                break;
        }
	}
}

bool Graphics::WindowIsOpen() const
{
	return m_Window.isOpen();
}

void Graphics::DrawEntities()
{
    Map* l_Map = m_MapManager->GetActualMap();

    std::map<TypeUnit, std::map<uint16, Unit*>>* l_ListUnitZone = l_Map->GetListUnitZone();

    /// Draw Entities
    for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_MapListUnit : (*l_ListUnitZone))
    {
        for (std::pair<uint16, Unit*> l_UnitPair : l_MapListUnit.second)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            /// SKIN PART
            uint8 l_SpriteNb = (l_Unit->GetOrientation() * MAX_MOVEMENT_POSITION) + l_Unit->GetMovementHandler()->GetMovementPosition();
            SkinSprite l_SkinSprite = (*m_SkinsManager->GetSkinSprite(l_Unit->GetSkinID(), l_SpriteNb));
            l_SkinSprite.setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
            l_SkinSprite.setColor(sf::Color(255, 255, 255, l_Unit->GetOpacity()));
            l_SkinSprite.setPosition((float)l_Unit->GetPosX(), (float)l_Unit->GetPosY() - l_Unit->GetRealSizeY());
            m_Window.draw(l_SkinSprite);

            /// TALK
            if (!l_Unit->GetTalk().empty())
            {
                sf::Text l_Text(l_Unit->GetTalk(), *g_Font, SIZE_TALK_FONT);

                TileSprite l_Sprite = m_InterfaceManager->GetField(l_Text.getGlobalBounds().width, (float)g_Font->getLineSpacing(l_Text.getCharacterSize()));
                l_Sprite.setPosition((float)l_Unit->GetPosX() + (l_Unit->GetRealSizeX() / 2) - (l_Text.getGlobalBounds().width / 2), (float)l_Unit->GetPosY() - l_Unit->GetRealSizeY() - l_Text.getGlobalBounds().height);
                m_Window.draw(l_Sprite);

                l_Text.setColor(sf::Color::White);
                l_Text.setPosition((float)l_Unit->GetPosX() + (l_Unit->GetRealSizeX() / 2) - (l_Text.getGlobalBounds().width / 2), (float)l_Unit->GetPosY() - l_Unit->GetRealSizeY() - l_Text.getGlobalBounds().height);
                m_Window.draw(l_Text);
            }

            /// NAME
            sf::Text l_Name(l_Unit->GetName(), *g_Font, SIZE_NAME_FONT);
            l_Name.setColor(sf::Color::White);
            l_Name.setPosition((float)l_Unit->GetPosX() + (l_Unit->GetRealSizeX() / 2) - (l_Name.getGlobalBounds().width / 2), (float)l_Unit->GetPosY());
            m_Window.draw(l_Name);
        }
    }
}

void Graphics::DrawMap()
{
	if (!m_Window.isOpen() || !m_MapManager->HasMap() || g_Player == nullptr)
		return;

	Map* l_Map = m_MapManager->GetActualMap();

	std::vector<std::vector<Case*>> l_SquareZone = l_Map->GetSquareZone(l_Map->GetSquareID(g_Player->GetCasePosX(), g_Player->GetCasePosY()));
	//printf("Square Acutal = %d\n", l_Map->GetSquareID(m_MapManager->GetPosX() / TILE_SIZE, m_MapManager->GetPosY() / TILE_SIZE));
	if (l_SquareZone.empty())
		return;

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
	}

    DrawEntities();

    /// Level 2 to 4
    for (std::vector<std::vector<Case*>>::iterator l_It = l_SquareZone.begin(); l_It != l_SquareZone.end(); ++l_It)
    {
        std::vector<Case*> l_Square = (*l_It);
        for (std::vector<Case*>::iterator l_It2 = l_Square.begin(); l_It2 != l_Square.end(); ++l_It2)
        {
            for (uint8 l_LevelNb = 2; l_LevelNb < 4; ++l_LevelNb)
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX() * TILE_SIZE, (float)(*l_It2)->GetPosY() * TILE_SIZE);
                m_Window.draw(*l_TileSprite);
            }
        }
    }
}

void Graphics::DrawInterface()
{
    if (!m_Window.isOpen() || !m_MapManager->HasMap() || g_Player == nullptr)
        return;

    m_InterfaceManager->Draw(m_Window);
}


void Graphics::UpdateWindow()
{
    Clear();
    if (g_Player != nullptr)
        m_View.setCenter((float)g_Player->GetPosX() + (g_Player->GetRealSizeX() / 2), (float)g_Player->GetPosY() - (g_Player->GetRealSizeY() / 2));
    m_Window.setView(m_View);
    DrawMap();
    m_Window.setView(m_ViewInterface);
    DrawInterface();
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
