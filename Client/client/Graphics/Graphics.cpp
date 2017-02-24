#include "Graphics.hpp"
#include "../Global.hpp"

Graphics::Graphics(MapManager* p_MapManager, InterfaceManager* p_InterfaceManager, Events* p_Events) :
    m_MapManager(p_MapManager),
    m_InterfaceManager(p_InterfaceManager),
    m_Events(p_Events),
    m_Thread(&Graphics::DrawLoop, this)
{
	m_TileSet = nullptr;
    m_VisualManager = nullptr;
    m_Clock = new ClockHandler();
}

Graphics::~Graphics()
{
}

void Graphics::DrawLoop()
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


bool Graphics::LoadTexture()
{
    std::string l_FileSystemName = "castbarmini.png";
    if (!m_CastBarTexture.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg Cast Bar Mini Failed\n");
    return true;
}

bool Graphics::CreateWindow(uint32 p_X, uint32 p_Y, float p_Zoom)
{
	m_Window.create(sf::VideoMode(p_X, p_Y), NAME_WINDOW);
	m_View = m_Window.getDefaultView();
    m_ViewFont = m_Window.getDefaultView();
    m_ViewInterface = m_Window.getDefaultView();
	m_View.zoom(p_Zoom);
	m_Window.setView(m_View);
    //m_Window.setFramerateLimit(60);
    m_Window.setVerticalSyncEnabled(true);

	m_TileSet = new TileSet();
	m_TileSet->BuildSprites();

    m_VisualManager = new VisualManager();
    if (!m_VisualManager->LoadSkins())
        return false;
    if (!m_VisualManager->LoadVisuals())
        return false;
    if (!LoadTexture())
        return false;
    return true;
}

void Graphics::CheckEvent()
{
	sf::Event l_Event;
    m_Events->Update();
	while (m_Window.pollEvent(l_Event))
	{
        switch(l_Event.type)
        {
            case sf::Event::Closed: ///< Request for closing the window
                m_Window.close();
                break;
           
            case sf::Event::Resized: ///< Resize Window
                /*
                std::cout << "new width: " << event.size.width << std::endl;
                std::cout << "new height: " << event.size.height << std::endl;
                */
                break;
                /// Reset KeyPress queue when lost focus
            default:
                m_InterfaceManager->ManageEvent(l_Event);
                break;
        }
	}
}

bool Graphics::WindowIsOpen() const
{
	return m_Window.isOpen();
}

Position Graphics::GetCenterPositionOnUnit(Unit* p_Unit, sf::Sprite* p_Sprite)
{
    int16 l_DiffSizeX = (p_Sprite->getTextureRect().width - p_Unit->GetSizeX()) / 2;
    int16 l_DiffSizeY = (p_Sprite->getTextureRect().height - p_Unit->GetSizeY()) / 2;

    Position l_Pos;
    l_Pos.x = p_Unit->GetPosXAtIntant() - l_DiffSizeX;
    l_Pos.y = p_Unit->GetPosYAtIntant() - l_DiffSizeY;
    return l_Pos;
}

void Graphics::DrawUnitDetails(Unit* p_Unit)
{
    if (p_Unit == nullptr)
        return;

    /// Set view to don t have a zoom on text
    m_Window.setView(m_ViewFont);

    std::vector<std::pair<DamageInfo, uint32>> l_DamageLogHistory = p_Unit->GetDamageLog();
    /// LOG DAMAGE
    for (std::pair<DamageInfo, uint32> l_DamageLog : l_DamageLogHistory)
    {
        int8 l_LogToShow = l_DamageLog.first.m_Damage;
        if (l_LogToShow < 0)
            l_LogToShow *= -1;
        std::string l_DmgStr = l_DamageLog.first.m_Result == DamageResult::Miss ? "Miss" : std::to_string(l_LogToShow);
        sf::Text l_Text(l_DmgStr, *g_Font, SIZE_TALK_FONT);

        if (l_DamageLog.first.m_Damage >= 0)
            l_Text.setColor(sf::Color(255, 66, 66));
        else
            l_Text.setColor(sf::Color(164, 255, 6));
        sf::Vector2f v1(p_Unit->GetPosXAtIntant(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 6 - 4 - ((MAX_HISTORY_LOG_TIME - l_DamageLog.second) / 100000));
        sf::Vector2f l_Coord = m_Window.mapCoordsToPixelFloat(v1, m_View);
        l_Text.setPosition((l_Coord.x - (l_Text.getGlobalBounds().width / 2)), l_Coord.y);
        m_Window.draw(l_Text);
    }
    /// TALK
    if (!p_Unit->GetTalk().empty())
    {
        sf::Text l_Text(p_Unit->GetTalk(), *g_Font, SIZE_TALK_FONT);
        sf::Vector2i l_FieldSize = m_InterfaceManager->TextSplitToFit(SIZE_TALK_TEXT_SIZE, l_Text);

        TileSprite l_Sprite = m_InterfaceManager->GetField(l_FieldSize.x + 8, l_FieldSize.y + 10);
        sf::Vector2f v1(p_Unit->GetPosXAtIntant() + (p_Unit->GetSizeX() / 4), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 4 - (l_FieldSize.y / 2));
        sf::Vector2f l_Coord = m_Window.mapCoordsToPixelFloat(v1, m_View);
        l_Sprite.setPosition((l_Coord.x - ((l_FieldSize.x + 8) / 2)) - (p_Unit->GetSizeX() / 2), l_Coord.y);
        m_Window.draw(l_Sprite);

        l_Text.setColor(sf::Color::White);
        sf::Vector2f v12(p_Unit->GetPosXAtIntant(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY());
        l_Coord = m_Window.mapCoordsToPixelFloat(v1, m_View);
        l_Text.setPosition((l_Coord.x - (l_FieldSize.x / 2)) - (p_Unit->GetSizeX() / 2), l_Coord.y);
        m_Window.draw(l_Text);
    }

    /// NAME
    if (p_Unit->IsPlayer())
    {
        sf::Text l_Name(p_Unit->GetName(), *g_Font, SIZE_NAME_FONT);
        uint8 l_Color = 255.0f / 100.0f * p_Unit->GetResourceNb(eResourceType::Alignment);
        l_Name.setColor(sf::Color(l_Color, l_Color, l_Color, 255));
        sf::Vector2f l_View(p_Unit->GetPosXAtIntant(), p_Unit->GetPosYAtIntant());
        sf::Vector2f l_Coord = m_Window.mapCoordsToPixelFloat(l_View, m_View);
        l_Name.setPosition((l_Coord.x - (l_Name.getGlobalBounds().width / 2.0f)), l_Coord.y);
        m_Window.draw(l_Name);
    }

    /// Reset the view
    m_Window.setView(m_View);

    /// CAST BAR
    if (p_Unit != g_Player && p_Unit->GetCastPct() > 0)
    {
        TileSprite l_CastBar;
        l_CastBar.setTexture(m_CastBarTexture);
        l_CastBar.setTextureRect(sf::IntRect(0, 0, (m_CastBarTexture.getSize().x / 100.0f) * p_Unit->GetCastPct(), m_CastBarTexture.getSize().y));
        l_CastBar.setPosition(p_Unit->GetPosXAtIntant() - (p_Unit->GetSizeX() / 2), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY());
        m_Window.draw(l_CastBar);
    }
}

void Graphics::DrawWorldObjects(std::map<uint32, std::vector<WorldObject*> > *p_ListWorldObjectsByZ)
{
    for (std::map<uint32, std::vector<WorldObject*> >::iterator l_It = p_ListWorldObjectsByZ->begin(); l_It != p_ListWorldObjectsByZ->end(); ++l_It)
    {
        for (auto l_WorldObject : (*l_It).second)
        {
            if (l_WorldObject == nullptr)
                continue;

            if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
                l_WorldObject->GetSprite()->setPosition(l_WorldObject->GetPosXAtIntant() - (l_WorldObject->GetSizeX() / 2), l_WorldObject->GetPosYAtIntant() - l_WorldObject->GetSizeY());
            else
                l_WorldObject->GetSprite()->setPosition(l_WorldObject->GetPosXAtIntant(), l_WorldObject->GetPosYAtIntant());
            m_Window.draw(*l_WorldObject->GetSprite());

            if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
            {
                Unit* l_Unit = l_WorldObject->ToUnit();
                std::vector<VisualEffect>  *l_VisualsEffect = l_Unit->GetVisualsEffect();

                /// VISUAL EFFECT
                for (std::vector<VisualEffect>::iterator l_It = l_VisualsEffect->begin(); l_It != l_VisualsEffect->end(); ++l_It)
                {
                    SkinSprite* l_SkinSprite = m_VisualManager->GetVisualSprite((*l_It).GetType(), (*l_It).GetID(), (*l_It).GetFrame());
                    if (l_SkinSprite)
                    {
                        l_SkinSprite->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
                        Position l_Pos = GetCenterPositionOnUnit(l_Unit, l_SkinSprite);
                        l_SkinSprite->setPosition(l_Pos.x - (l_Unit->GetSizeX() / 2), l_Pos.y - l_Unit->GetSizeY());
                        m_Window.draw(*l_SkinSprite);
                    }
                }
            }
        }
    }
}

void Graphics::DrawWorldObjectsTxt(std::map<uint32, std::vector<WorldObject*> > *p_ListWorldObjectsByZ)
{
    for (std::map<uint32, std::vector<WorldObject*> >::iterator l_It = p_ListWorldObjectsByZ->begin(); l_It != p_ListWorldObjectsByZ->end(); ++l_It)
    {
        for (auto l_WorldObject : (*l_It).second)
        {
            if (l_WorldObject == nullptr)
                continue;

            if (!l_WorldObject->GetType() == TypeWorldObject::UNIT)
                continue;

            /// Draw specificity of Unit (name, text, dmg ...)
            DrawUnitDetails(l_WorldObject->ToUnit());
        }
    }
}


bool Graphics::IsInRayWindow(WorldObject* p_Center, WorldObject* p_Obj)
{
    uint8 l_TotalCaseXWindow = X_WINDOW / TILE_SIZE;
    uint8 l_TotalCaseYWindow = Y_WINDOW / TILE_SIZE;

    l_TotalCaseXWindow *= ZOOM_FACTOR;
    l_TotalCaseYWindow *= ZOOM_FACTOR;
    int16 l_CaseDiff = 0;
    if (p_Center->GetPosXCase() > p_Obj->GetPosXCase())
        l_CaseDiff = p_Center->GetPosXCase() - p_Obj->GetPosXCase();
    else
        l_CaseDiff = p_Obj->GetPosXCase() - p_Center->GetPosXCase() - 1;
    
    if (l_CaseDiff > l_TotalCaseXWindow / 2)
        return false;

    if (p_Center->GetPosYCase() > p_Obj->GetPosYCase())
        l_CaseDiff = p_Center->GetPosYCase() - p_Obj->GetPosYCase() - 2;
    else
        l_CaseDiff = p_Obj->GetPosYCase() - p_Center->GetPosYCase();

    if (l_CaseDiff > l_TotalCaseYWindow / 2)
        return false;
    return true;
}

void Graphics::DrawMap()
{
	if (!m_Window.isOpen() || !m_MapManager->HasMap() || g_Player == nullptr)
		return;

	Map* l_Map = m_MapManager->GetActualMap();
    std::map<TypeUnit, std::map<uint16, Unit*> >* l_ListUnitZone = l_Map->GetListUnitZone();
    std::map<uint32, std::vector<WorldObject*> > l_ListWorldObjectByZ;

	std::vector<std::vector<Case*>> l_SquareZone = l_Map->GetSquareZone(l_Map->GetSquareID(g_Player->GetPosXCase(), g_Player->GetPosYCase()));
	//printf("Square Acutal = %d\n", l_Map->GetSquareID(m_MapManager->GetPosX() / TILE_SIZE, m_MapManager->GetPosY() / TILE_SIZE));
	if (l_SquareZone.empty())
		return;

    /// Level 1 and 2
    /// Level 5 and 6
	for (std::vector<std::vector<Case*>>::iterator l_It = l_SquareZone.begin(); l_It != l_SquareZone.end(); ++l_It)
	{
		std::vector<Case*> l_Square = (*l_It);
		for (std::vector<Case*>::iterator l_It2 = l_Square.begin(); l_It2 != l_Square.end(); ++l_It2)
		{
            if (!IsInRayWindow(g_Player, (*l_It2)))
                continue;

            for (uint8 l_LevelNb = 0; l_LevelNb < 2; ++l_LevelNb)
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX(), (float)(*l_It2)->GetPosY());
                m_Window.draw(*l_TileSprite);
            }
            for (uint8 l_LevelNb = 4; l_LevelNb < 6; ++l_LevelNb) ///< Couches specials
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX(), (float)(*l_It2)->GetPosY());
                (*l_It2)->SetSprite(l_TileSprite);
                if (l_LevelNb == 4) ///< + GetSizeY because tile of Unit are set to be 0 at foot cf : DrawWorldObjects UNIT
                    l_ListWorldObjectByZ[(*l_It2)->GetPosY() + (*l_It2)->GetSizeY()].push_back(*l_It2);
                else
                    l_ListWorldObjectByZ[(*l_It2)->GetPosY() + ((*l_It2)->GetSizeY() * 2)].push_back(*l_It2);
            }
		}
	}
    
    std::map<uint32, std::vector<WorldObject*> > l_ListUnitByZ;
    for (std::pair<TypeUnit, std::map<uint16, Unit*>> l_MapListUnit : (*l_ListUnitZone))
    {
        for (std::pair<uint16, Unit*> l_UnitPair : l_MapListUnit.second)
        {
            Unit* l_Unit = l_UnitPair.second;

            if (l_Unit == nullptr)
                continue;

            if (!IsInRayWindow(g_Player, l_Unit))
                continue;

            MovementHandler* l_MovementHandler = l_Unit->GetMovementHandler();
            uint8 l_SpriteNb = (l_Unit->GetOrientation() * MAX_MOVEMENT_POSITION) + l_MovementHandler->GetMovementPosition();
            if (l_MovementHandler->IsInAttack())
                l_SpriteNb += (MAX_MOVEMENT_POSITION * Orientation::MAX);

            SkinSprite* l_SkinSprite = m_VisualManager->GetVisualSprite(eVisualType::VisualSkin, l_Unit->GetSkinID(), l_SpriteNb);
            l_SkinSprite->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
            if (l_Unit->IsPlayer())
                l_SkinSprite->setColor(sf::Color(255, 255, 255, l_Unit->GetOpacity()));
            else
                l_SkinSprite->setColor(sf::Color(255, 255, 255, 255));

            l_Unit->SetSprite(l_SkinSprite);

            l_ListWorldObjectByZ[l_Unit->GetPosYAtIntant()].push_back(l_Unit);
        }
    }
    DrawWorldObjects(&l_ListWorldObjectByZ);
    /// Level 3 and 4
    for (std::vector<std::vector<Case*>>::iterator l_It = l_SquareZone.begin(); l_It != l_SquareZone.end(); ++l_It)
    {
        std::vector<Case*> l_Square = (*l_It);
        for (std::vector<Case*>::iterator l_It2 = l_Square.begin(); l_It2 != l_Square.end(); ++l_It2)
        {
            if (!IsInRayWindow(g_Player, (*l_It2)))
                continue;

            for (uint8 l_LevelNb = 2; l_LevelNb < 4; ++l_LevelNb)
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX(), (float)(*l_It2)->GetPosY());
                m_Window.draw(*l_TileSprite);
            }
        }
    }
    DrawWorldObjectsTxt(&l_ListWorldObjectByZ);
}

void Graphics::DrawInterface()
{
    if (!m_Window.isOpen())
        return;

    m_InterfaceManager->Draw(m_Window);
}

void Graphics::UpdateInterface(sf::Time p_Diff)
{
    if (!m_Window.isOpen() || !m_MapManager->HasMap() || g_Player == nullptr)
        return;

    m_InterfaceManager->Update(p_Diff);
}

void Graphics::UpdateWindow(sf::Time p_Diff)
{
    if (g_Player != nullptr)
        m_View.setCenter(g_Player->GetMovementHandler()->GetPosXAtIntant(), g_Player->GetMovementHandler()->GetPosYAtIntant() - (float)(g_Player->GetSizeY() / 2));
    UpdateInterface(p_Diff);
    m_Window.setView(m_View);
    Clear();
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
