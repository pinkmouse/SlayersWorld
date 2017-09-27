#include "Graphics.hpp"
#include "../Global.hpp"
#include "../Entities/AnimationUnit.hpp"
#include "../Graphics/Interface/MenuWardrobe.hpp"

#define MINIBAR_GREEN 0
#define MINIBAR_YELLOW 1
#define MINIBAR_RED 2
#define MINIBAR_PURPLE 3
#define MINIBAR_BLUE 4

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
    std::string l_FileSystemName = "barminis.png";
    if (!m_BarMinis.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg Bar Minis Failed\n");
    l_FileSystemName = "uimini.png";
    if (!m_UiMiniTexture.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg UI Mini Failed\n");
    return true;
}

void Graphics::SetTileSet(const std::string & p_FileName)
{
    delete m_TileSet;
    m_TileSet = new TileSet(p_FileName);
    m_TileSet->BuildSprites();
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

    m_VisualManager = new VisualManager();
    if (!m_VisualManager->LoadSkins())
        return false;
    if (!m_VisualManager->LoadVisuals())
        return false;
    if (!LoadTexture())
        return false;

    /// Menu need visual manager to show skins
    m_InterfaceManager->SetVisualManager(m_VisualManager);
    m_InterfaceManager->GetMenuManager()->SetVisualManager(m_VisualManager);
    /*MenuWardrobe* l_Wardrobe = reinterpret_cast<MenuWardrobe*>(m_InterfaceManager->GetMenuManager()->GetMenu(eMenuType::WardrobeMenu));
    l_Wardrobe->AddSkin(49, "Test1");
    l_Wardrobe->AddSkin(50, "Test2");
    l_Wardrobe->AddSkin(51, "Test3");
    l_Wardrobe->AddSkin(52, "Test4");
    l_Wardrobe->AddSkin(53, "Test5");
    l_Wardrobe->AddSkin(54, "Test6");*/
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

TileSprite Graphics::GetBarMini(uint8 p_ID, uint8 p_Pct)
{
    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_BarMinis);
    l_TileSprite.setTextureRect(sf::IntRect(0, p_ID * 4, (m_BarMinis.getSize().x / 100.0f) * p_Pct, 4));
    return l_TileSprite;
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

    if (p_Unit->IsDynamicObject())
        return;

    int8 l_OffsetLifeBar = -4;
    uint8 l_LifeBarColor = MINIBAR_RED;

    if (p_Unit->GetIsInGroup())
    {
        l_OffsetLifeBar = -6;
        l_LifeBarColor = MINIBAR_GREEN;
    }
    /// LIFE BAR
    if (p_Unit != g_Player && p_Unit->IsPlayer())
    {
        TileSprite l_LifeBar = GetBarMini(l_LifeBarColor, p_Unit->GetResourceNb(eResourceType::Health));
        l_LifeBar.setPosition(p_Unit->GetPosXAtIntant() - (p_Unit->GetSizeX() / 2) + p_Unit->GetPosXOffset(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 4 + l_OffsetLifeBar + p_Unit->GetPosYOffset());
        m_Window.draw(l_LifeBar);
    }

    /// CAST BAR
    if (p_Unit != g_Player && p_Unit->GetCastPct() > 0)
    {
        TileSprite l_CastBar = GetBarMini(MINIBAR_YELLOW, p_Unit->GetCastPct());
        l_CastBar.setPosition(p_Unit->GetPosXAtIntant() - (p_Unit->GetSizeX() / 2) + p_Unit->GetPosXOffset(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 4 + p_Unit->GetPosYOffset());
        m_Window.draw(l_CastBar);
    }

    /// UI GROUP
    if (p_Unit != g_Player && p_Unit->GetIsInGroup())
    {
        TileSprite l_UiGroup;
        l_UiGroup.setTexture(m_UiMiniTexture);
        l_UiGroup.setTextureRect(sf::IntRect(0, 0, m_UiMiniTexture.getSize().x, m_UiMiniTexture.getSize().y));
        l_UiGroup.setPosition(p_Unit->GetPosXAtIntant() - (p_Unit->GetSizeX() / 2) - 6 + p_Unit->GetPosXOffset(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 12 + p_Unit->GetPosYOffset());
        m_Window.draw(l_UiGroup);
    }

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
        sf::Vector2f l_V1(p_Unit->GetPosXAtIntant() + p_Unit->GetPosXOffset(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 6 - 4 - ((MAX_HISTORY_LOG_TIME - l_DamageLog.second) / 100000) + p_Unit->GetPosYOffset());
        sf::Vector2f l_Coord = CoordFromViewToView(l_V1, m_View, m_ViewInterface);
        l_Text.setPosition((l_Coord.x - (l_Text.getGlobalBounds().width / 2)), l_Coord.y);
        m_Window.draw(l_Text);
    }
    /// TALK
    if (!p_Unit->GetTalk().empty())
    {
        sf::Text l_Text(p_Unit->GetTalk(), *g_Font, SIZE_TALK_FONT);
        sf::Vector2i l_FieldSize = m_InterfaceManager->TextSplitToFit(SIZE_TALK_TEXT_SIZE, l_Text);

        sf::Vector2f l_V1(p_Unit->GetPosXAtIntant() + (p_Unit->GetSizeX() / 4) + p_Unit->GetPosXOffset(), p_Unit->GetPosYAtIntant() - p_Unit->GetSizeY() - 4 - (l_FieldSize.y / 2) + p_Unit->GetPosYOffset());
        sf::Vector2f l_Coord = CoordFromViewToView(l_V1, m_View, m_ViewInterface);
        m_InterfaceManager->DrawField(m_Window, (l_Coord.x - ((l_FieldSize.x + 8) / 2)) - (p_Unit->GetSizeX() / 2), l_Coord.y, l_FieldSize.x + 14, l_FieldSize.y + 14);


        l_Text.setColor(sf::Color::White);
        l_Text.setPosition((l_Coord.x - (l_FieldSize.x / 2)) - (p_Unit->GetSizeX() / 2) + 2.0f, l_Coord.y + 2.0f);
        m_Window.draw(l_Text);
    }

    /// Reset the view
    m_Window.setView(m_View);
}

void Graphics::DrawWorldObjects(std::map<uint32, std::vector<WorldObject*> > *p_ListWorldObjectsByZ)
{
    for (std::map<uint32, std::vector<WorldObject*> >::iterator l_It = p_ListWorldObjectsByZ->begin(); l_It != p_ListWorldObjectsByZ->end(); ++l_It)
    {
        for (auto l_WorldObject : (*l_It).second)
        {
            if (l_WorldObject == nullptr)
                continue;

            SkinSprite* l_MountSkin = nullptr;

            float l_OffsetX = 0;
            float l_OffsetY = 0;

            /// MOUNT
            if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
            {
                Unit* l_Unit = l_WorldObject->ToUnit();
                l_OffsetX = l_Unit->GetPosXOffset();
                l_OffsetY = l_Unit->GetPosYOffset();

                if (l_Unit->GetMount() >= 0)
                {
                    uint8 l_SpriteNb = (l_Unit->GetOrientation() * MAX_MOVEMENT_POSITION) + l_Unit->GetMovementHandler()->GetMovementPosition();
                    l_MountSkin = m_VisualManager->GetVisualSprite(eVisualType::VisualSkinMount, l_Unit->GetMount(), l_SpriteNb);
                    if (l_MountSkin)
                    {
                        l_MountSkin->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
                        Position l_Pos = GetCenterPositionOnUnit(l_Unit, l_MountSkin);
                        l_MountSkin->setPosition(l_Pos.x - (l_Unit->GetSizeX() / 2), l_Pos.y - l_Unit->GetSizeY());
                        if (l_Unit->GetOrientation() != Orientation::Down && l_Unit->GetOrientation() != Orientation::Up)
                            m_Window.draw(*l_MountSkin);
                    }
                }
            }

            if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
            {
                Unit* l_Unit = l_WorldObject->ToUnit();
                std::map< std::pair<TypeUnit, uint16>, std::map<uint8, VisualEffect> >  *l_VisualsEffect = l_Unit->GetVisualsEffect();

                /// VISUAL EFFECT UNDER
                for (std::map< std::pair<TypeUnit, uint16>, std::map<uint8, VisualEffect> >::iterator l_It = l_VisualsEffect->begin(); l_It != l_VisualsEffect->end(); ++l_It)
                {
                    for (std::map<uint8, VisualEffect>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end(); l_Itr++)
                    {
                        if (!(*l_Itr).second.IsUnder())
                            continue;

                        SkinSprite* l_SkinSprite = m_VisualManager->GetVisualSprite((*l_Itr).second.GetType(), (*l_Itr).second.GetID(), l_Unit->GetOrientation() * (*l_Itr).second.GetMaxFrame() + (*l_Itr).second.GetFrame());
                        if (l_SkinSprite)
                        {
                            l_SkinSprite->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
                            Position l_Pos = l_Unit->GetPosition();
                            if (!l_Unit->IsDynamicObject())
                                GetCenterPositionOnUnit(l_Unit, l_SkinSprite);
                            l_SkinSprite->setPosition(l_Pos.x + l_OffsetX - (l_SkinSprite->getGlobalBounds().width / 2), l_Pos.y - (l_Unit->GetSizeY() / 2) + l_OffsetY - (l_SkinSprite->getGlobalBounds().height / 2));
                            m_Window.draw(*l_SkinSprite);
                        }
                    }
                }
            }

            sf::Sprite* l_Sprite = l_WorldObject->GetSprite();

            if (l_Sprite != nullptr)
            {
            /// UNIT
                if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
                    l_Sprite->setPosition(l_WorldObject->GetPosXAtIntant() - (l_WorldObject->GetSizeX() / 2) + l_OffsetX, l_WorldObject->GetPosYAtIntant() - l_WorldObject->GetSizeY() + l_OffsetY);
                else
                    l_Sprite->setPosition(l_WorldObject->GetPosXAtIntant(), l_WorldObject->GetPosYAtIntant());

                sf::Sprite l_SpriteUnp = *l_WorldObject->GetSprite();

                l_SpriteUnp.setColor(sf::Color(255, 255, 255, l_WorldObject->GetOpacity()));
                m_Window.draw(l_SpriteUnp);
            }
            if (l_WorldObject->GetType() == TypeWorldObject::UNIT)
            {
                Unit* l_Unit = l_WorldObject->ToUnit();
                std::map< std::pair<TypeUnit, uint16>, std::map<uint8, VisualEffect> >  *l_VisualsEffect = l_Unit->GetVisualsEffect();

                if (l_MountSkin != nullptr && (l_Unit->GetOrientation() == Orientation::Down || l_Unit->GetOrientation() == Orientation::Up))
                    m_Window.draw(*l_MountSkin);

                /// VISUAL EFFECT
                for (std::map< std::pair<TypeUnit, uint16>, std::map<uint8, VisualEffect> >::iterator l_It = l_VisualsEffect->begin(); l_It != l_VisualsEffect->end(); ++l_It)
                {
                    for (std::map<uint8, VisualEffect>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end(); l_Itr++)
                    {
                        if ((*l_Itr).second.IsUnder())
                            continue;

                        SkinSprite* l_SkinSprite = m_VisualManager->GetVisualSprite((*l_Itr).second.GetType(), (*l_Itr).second.GetID(), l_Unit->GetOrientation() * (*l_Itr).second.GetMaxFrame() + (*l_Itr).second.GetFrame());
                        if (l_SkinSprite)
                        {
                            l_SkinSprite->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
                            Position l_Pos = l_Unit->GetPosition();
                            if (!l_Unit->IsDynamicObject())
                                GetCenterPositionOnUnit(l_Unit, l_SkinSprite);
                            l_SkinSprite->setPosition(l_Pos.x + l_OffsetX - (l_SkinSprite->getGlobalBounds().width / 2), l_Pos.y - (l_Unit->GetSizeY() / 2) + l_OffsetY - (l_SkinSprite->getGlobalBounds().height / 2));
                            m_Window.draw(*l_SkinSprite);
                        }
                    }
                }
                m_Window.setView(m_ViewFont);
                /// NAME
                if (l_Unit->IsPlayer())
                {
                    sf::Text l_Name(l_Unit->GetName(), *g_Font, SIZE_NAME_FONT);
                    uint8 l_Color = 255.0f / 100.0f * l_Unit->GetResourceNb(eResourceType::Alignment);
                    l_Name.setColor(sf::Color(255.0f, l_Color, l_Color, 255));
                    sf::Vector2f l_View(l_Unit->GetPosXAtIntant(), l_Unit->GetPosYAtIntant());

                    sf::Vector2f l_Coord = CoordFromViewToView(l_View, m_View, m_ViewInterface);

                    //sf::Vector2f l_Coord = m_Window.mapCoordsToPixelFloat(l_View, m_View);
                    l_Name.setPosition((l_Coord.x - (l_Name.getGlobalBounds().width / 2.0f)), l_Coord.y);
                    m_Window.draw(l_Name);
                }
                m_Window.setView(m_View);

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

    if (m_TileSet == nullptr || m_TileSet->GetFileName() != m_MapManager->GetActualMap()->GetChipsetFileName())
    {
        SetTileSet(m_MapManager->GetActualMap()->GetChipsetFileName());
        m_InterfaceManager->SetIsLoading(false);
    }

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
                if (l_TileSprite == nullptr)
                    continue;
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX(), (float)(*l_It2)->GetPosY());
                m_Window.draw(*l_TileSprite);
            }
            for (uint8 l_LevelNb = 4; l_LevelNb < 6; ++l_LevelNb) ///< Couches specials
            {
                int16 l_TileID = (*l_It2)->GetTile(l_LevelNb);

                if (l_TileID < 0)
                    continue;

                TileSprite* l_TileSprite = m_TileSet->GetTileSprite(l_TileID);
                if (l_TileSprite == nullptr)
                    continue;
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

            if (l_Unit->GetSkinID() < 0)
                continue;

            MovementHandler* l_MovementHandler = l_Unit->GetMovementHandler();
            uint8 l_SpriteNb = (l_Unit->GetOrientation() * MAX_MOVEMENT_POSITION) + l_MovementHandler->GetMovementPosition();
            if (l_Unit->GetMount() >= 0)
                l_SpriteNb = (l_Unit->GetOrientation() * MAX_MOVEMENT_POSITION);
            if (l_MovementHandler->IsInAttack())
                l_SpriteNb += (MAX_MOVEMENT_POSITION * Orientation::MAX);

            SkinSprite* l_SkinSprite = nullptr;
            if (l_Unit->GetType() == TypeUnit::CREATURE || l_Unit->GetType() == TypeUnit::PLAYER)
                l_SkinSprite = m_VisualManager->GetVisualSprite(eVisualType::VisualSkin, l_Unit->GetSkinID(), l_SpriteNb);
            else if (l_Unit->GetType() == TypeUnit::ANIMATIONUNIT)
                l_SkinSprite = m_VisualManager->GetVisualSprite(eVisualType::VisualAnimationUnit, l_Unit->GetSkinID(), GetFrameNbForAnimationUnit(l_Unit));
            /*else
                l_SkinSprite = m_VisualManager->GetVisualSprite(eVisualType::VisualGob, l_Unit->GetSkinID(), l_SpriteNb);*/

            if (l_SkinSprite != nullptr)
            {
                l_SkinSprite->setScale(sf::Vector2f(l_Unit->GetSkinZoomFactor(), l_Unit->GetSkinZoomFactor()));
                l_Unit->SetSprite(l_SkinSprite);
            }
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
                if (l_TileSprite == nullptr)
                    continue;
                l_TileSprite->setPosition((float)(*l_It2)->GetPosX(), (float)(*l_It2)->GetPosY());
                TileSprite l_UnPtrTileSprite = *l_TileSprite;
                Case *l_ActualCase = l_Map->GetCase(g_Player->GetPosX(), g_Player->GetPosY());
                if (g_Player && l_ActualCase && (l_ActualCase->GetID() == (*l_It2)->GetID()
                    || l_ActualCase->GetID() - l_Map->GetSizeX() == (*l_It2)->GetID()
                    || l_ActualCase->GetID() - l_Map->GetSizeX() - 1 == (*l_It2)->GetID()
                    || l_ActualCase->GetID() - l_Map->GetSizeX() + 1 == (*l_It2)->GetID()
                    || l_ActualCase->GetID() + 1 == (*l_It2)->GetID()
                    || l_ActualCase->GetID() - 1 == (*l_It2)->GetID()))
                    l_UnPtrTileSprite.setColor(sf::Color(255, 255, 255, 170));
                m_Window.draw(l_UnPtrTileSprite);
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

sf::Vector2f Graphics::CoordFromViewToView(const sf::Vector2f & p_ActualCoord, const sf::View & p_View1, const sf::View & p_View2)
{
    return m_Window.mapPixelToCoords(m_Window.mapCoordsToPixel(p_ActualCoord, p_View1), p_View2);
}

uint8 Graphics::GetFrameNbForAnimationUnit(Unit* p_Unit)
{
    uint8 l_Frame = 0;
    AnimationUnit* l_AnimationUnit = p_Unit->ToAnimationUnit();
    if (l_AnimationUnit == nullptr)
        return l_Frame;

    if (l_AnimationUnit->IsInAnimation())
        l_Frame = 12 + l_AnimationUnit->GetFrame();
    else
    {
        switch (p_Unit->GetOrientation())
        {
        case Orientation::Left:
            l_Frame = 0;
            break;
        case Orientation::Right:
            l_Frame = 2;
            break;
        default:
            l_Frame = 1;
            break;
        }
    }
    return l_Frame;
}