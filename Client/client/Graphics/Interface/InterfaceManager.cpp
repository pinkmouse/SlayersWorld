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
}

TileSprite InterfaceManager::GetField(uint16 p_SizeX, uint16 p_SizeY)
{
    TileSprite l_TileSprite;
    l_TileSprite.setTexture(m_SystemTexture);
    l_TileSprite.setTextureRect(sf::IntRect(0, 0, FONT_FIELD_SIZE, FONT_FIELD_SIZE));
    l_TileSprite.setScale((float)p_SizeX / FONT_FIELD_SIZE, (float)p_SizeY / FONT_FIELD_SIZE);
    return l_TileSprite;
}

void InterfaceManager::Draw(Window & p_Window)
{
    if (m_Events == nullptr)
        return;

    if (m_WritingField->IsFieldOpen())
    {
        TileSprite l_Sprite = GetField(X_WINDOW, SIZE_FILED_TALK_Y);
        l_Sprite.setPosition(0, Y_WINDOW - SIZE_FILED_TALK_Y);
        p_Window.draw(l_Sprite);
        p_Window.draw(m_WritingField->GetText());
    }
}
