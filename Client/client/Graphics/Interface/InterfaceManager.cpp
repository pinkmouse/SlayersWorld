#include "InterfaceManager.hpp"
#include "../../Global.hpp"

InterfaceManager::InterfaceManager(Events* p_Events) :
    m_Events(p_Events),
    m_WritingField(new WritingField()),
    m_HistoryField(new HistoryField())
{
    m_Events->SetWritingField(m_WritingField);
    m_Events->SetHistoryField(m_HistoryField);
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
        printf("Load SystemImg Flask Failed\n");

    l_FileSystemName = "xp.png";
    if (!m_XpTexture.loadFromFile(IMG_FOLDER + l_FileSystemName))
        printf("Load SystemImg Xp Failed\n");
}

TileSprite InterfaceManager::GetField(uint16 p_SizeX, uint16 p_SizeY)
{
    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_SystemTexture);
    l_TileSprite.setTextureRect(sf::IntRect(0, 0, FONT_FIELD_SIZE, FONT_FIELD_SIZE));
    l_TileSprite.setScale((float)p_SizeX / FONT_FIELD_SIZE, (float)p_SizeY / FONT_FIELD_SIZE);
    return l_TileSprite;
}

TileSprite InterfaceManager::GetFlask(uint8 p_ID, bool p_Full, uint8 p_Pct)
{
    uint8 l_Full = 0;
    if (!p_Full)
        l_Full = 1;

    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_FlaskTexture);
    l_TileSprite.setTextureRect(sf::IntRect(FLASK_SIZE_X * ((p_ID * 2) + l_Full), 0, FLASK_SIZE_X, FLASK_SIZE_Y));
    return l_TileSprite;
}

TileSprite InterfaceManager::GetXpBar(bool p_Full, uint8 p_Pct)
{
    uint8 l_Full = 0;
    if (p_Full)
        l_Full = 1;

    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_XpTexture);
    l_TileSprite.setTextureRect(sf::IntRect(0, XP_BAR_SIZE_Y * l_Full, XP_BAR_SIZE_X, XP_BAR_SIZE_Y));
    return l_TileSprite;
}

HistoryField* InterfaceManager::GetHistoryField() const
{
    return m_HistoryField;
}

void InterfaceManager::Draw(Window & p_Window)
{
    if (m_Events == nullptr)
        return;

    /// Draw Flask Life
    TileSprite l_FlaskEmpty = GetFlask(0, false);
    l_FlaskEmpty.setPosition(0, Y_WINDOW - (FLASK_SIZE_Y * FLASK_SCALE) - (XP_BAR_SIZE_Y * XP_BAR_SCALE));
    l_FlaskEmpty.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_FlaskEmpty);
    TileSprite l_Flask = GetFlask(0, true);
    l_Flask.setTextureRect(sf::IntRect(0, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * (1.0f - ((float)g_Player->GetHealth() / 100.0f))) + FLASK_OFFSET_TOP, FLASK_SIZE_X, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f))));
    l_Flask.setPosition(0, Y_WINDOW - (XP_BAR_SIZE_Y * XP_BAR_SCALE) - (FLASK_OFFSET_BOTTOM * FLASK_SCALE) - (((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f)) * FLASK_SCALE));
    l_Flask.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_Flask);

    /// Draw Flask Mana
    l_FlaskEmpty = GetFlask(1, false);
    l_FlaskEmpty.setPosition(X_WINDOW - (FLASK_SIZE_X * FLASK_SCALE), Y_WINDOW - (FLASK_SIZE_Y * FLASK_SCALE) - (XP_BAR_SIZE_Y * XP_BAR_SCALE));
    l_FlaskEmpty.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_FlaskEmpty);
    l_Flask = GetFlask(1, true);
    l_Flask.setTextureRect(sf::IntRect(FLASK_SIZE_X * 2, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * (1.0f - ((float)g_Player->GetHealth() / 100.0f))) + FLASK_OFFSET_TOP, FLASK_SIZE_X, ((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f))));
    l_Flask.setPosition(X_WINDOW - (FLASK_SIZE_X * FLASK_SCALE), Y_WINDOW - (XP_BAR_SIZE_Y * XP_BAR_SCALE) - (FLASK_OFFSET_BOTTOM * FLASK_SCALE) - (((FLASK_SIZE_Y - FLASK_OFFSET_TOP - FLASK_OFFSET_BOTTOM) * ((float)g_Player->GetHealth() / 100.0f)) * FLASK_SCALE));
    l_Flask.setScale(FLASK_SCALE, FLASK_SCALE);
    p_Window.draw(l_Flask);

    /// Draw XP bar
    TileSprite l_XpBarEmpty = GetXpBar(false);
    l_XpBarEmpty.setPosition(0, Y_WINDOW - (XP_BAR_SIZE_Y * XP_BAR_SCALE));
    l_XpBarEmpty.setScale(XP_BAR_SCALE, XP_BAR_SCALE);
    p_Window.draw(l_XpBarEmpty);
    TileSprite l_XpBar = GetXpBar(true);
    l_XpBar.setTextureRect(sf::IntRect(0, XP_BAR_SIZE_Y, (XP_BAR_SIZE_X / 100.0f) * g_Player->GetXpPct(), XP_BAR_SIZE_Y));
    l_XpBar.setPosition(0, Y_WINDOW - (XP_BAR_SIZE_Y * XP_BAR_SCALE));
    l_XpBar.setScale(XP_BAR_SCALE, XP_BAR_SCALE);
    p_Window.draw(l_XpBar);

    /// Draw chat bar
    if (m_WritingField->IsFieldOpen())
    {
        TileSprite l_Sprite = GetField(X_WINDOW, SIZE_FILED_TALK_Y);
        l_Sprite.setPosition(0, Y_WINDOW - SIZE_FILED_TALK_Y);
        p_Window.draw(l_Sprite);
        p_Window.draw(m_WritingField->GetText());
    }
    /// Draw history
    if (m_HistoryField->IsFieldOpen())
    {
        TileSprite l_Sprite = GetField(X_WINDOW - (FLASK_SIZE_X * 2 * FLASK_SCALE), SIZE_FILED_TALK_Y * 5);
        l_Sprite.setPosition(FLASK_SIZE_X * FLASK_SCALE, Y_WINDOW - SIZE_FILED_TALK_Y - (SIZE_FILED_TALK_Y * 5));
        p_Window.draw(l_Sprite);
        for (uint8 i = 0; i < m_HistoryField->GetHistory().size(); ++i)
        {
            sf::Text l_Text = m_HistoryField->GetText();
            l_Text.setFont(*g_Font);
            l_Text.setPosition(FLASK_SIZE_X * FLASK_SCALE, Y_WINDOW - SIZE_FILED_TALK_Y - (SIZE_FILED_TALK_Y * (5 - i)));
            l_Text.setString(m_HistoryField->GetHistory()[i]);
            p_Window.draw(l_Text);
        }
        //p_Window.draw(m_WritingField->GetText());
    }
}
