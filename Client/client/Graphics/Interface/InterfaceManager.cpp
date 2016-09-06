#include "InterfaceManager.hpp"
#include "../../Global.hpp"

InterfaceManager::InterfaceManager(Events* p_Events) :
    m_Events(p_Events),
    m_WritingField(new WritingField())
{
    m_Events->SetWritingField(m_WritingField);
}


InterfaceManager::~InterfaceManager()
{
}

void InterfaceManager::Initialize()
{
    std::string l_FileSystemName = "system.png";
    if (!m_SystemTexture.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg Failed\n");

    l_FileSystemName = "flask.png";
    if (!m_FlaskTexture.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg Failed\n");
}

TileSprite InterfaceManager::GetField(uint16 p_SizeX, uint16 p_SizeY)
{
    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_SystemTexture);
    l_TileSprite.setTextureRect(sf::IntRect(0, 0, FONT_FIELD_SIZE, FONT_FIELD_SIZE));
    l_TileSprite.setScale((float)p_SizeX / FONT_FIELD_SIZE, (float)p_SizeY / FONT_FIELD_SIZE);
    return l_TileSprite;
}

TileSprite InterfaceManager::GetFlask(uint8 p_ID, bool p_full, uint8 p_Pct)
{
    uint8 l_Full = 0;
    if (!p_full)
        l_Full = 1;

    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_FlaskTexture);
    l_TileSprite.setTextureRect(sf::IntRect(FLASK_SIZE_X * ((p_ID * 2) + l_Full), 0, FLASK_SIZE_X, FLASK_SIZE_Y));
    return l_TileSprite;
}

void InterfaceManager::Draw(Window & p_Window)
{
    if (m_Events == nullptr)
        return;

    /// Draw Flask Life
    TileSprite l_FlaskEmpty = GetFlask(0, false);
    l_FlaskEmpty.setPosition(0, Y_WINDOW - (FLASK_SIZE_Y * FLASK_SCALE));
    l_FlaskEmpty.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_FlaskEmpty);
    TileSprite l_Flask = GetFlask(0, true);
    l_Flask.setTextureRect(sf::IntRect(0, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * (1.0f - ((float)g_Player->GetHealth() / 100.0f))) + FLASK_OFFSET_TOP, FLASK_SIZE_X, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f))));
    l_Flask.setPosition(0, Y_WINDOW - (FLASK_OFFSET_BOTTOM * FLASK_SCALE) - (((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f)) * FLASK_SCALE));
    l_Flask.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_Flask);

    /// Draw Flask Mana
    l_FlaskEmpty = GetFlask(1, false);
    l_FlaskEmpty.setPosition(X_WINDOW - (FLASK_SIZE_X * FLASK_SCALE), Y_WINDOW - (FLASK_SIZE_Y * FLASK_SCALE));
    l_FlaskEmpty.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_FlaskEmpty);
    l_Flask = GetFlask(1, true);
    l_Flask.setTextureRect(sf::IntRect(FLASK_SIZE_X * 2, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * (1.0f - ((float)g_Player->GetHealth() / 100.0f))) + FLASK_OFFSET_TOP, FLASK_SIZE_X, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f))));
    l_Flask.setPosition(X_WINDOW - (FLASK_SIZE_X * FLASK_SCALE), Y_WINDOW - (FLASK_OFFSET_BOTTOM * FLASK_SCALE) - (((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f)) * FLASK_SCALE));
    l_Flask.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_Flask);

    /// Draw chat bar
    if (m_WritingField->IsFieldOpen())
    {
        TileSprite l_Sprite = GetField(X_WINDOW, SIZE_FILED_TALK_Y);
        l_Sprite.setPosition(0, Y_WINDOW - SIZE_FILED_TALK_Y);
        p_Window.draw(l_Sprite);
        p_Window.draw(m_WritingField->GetText());
    }
}
