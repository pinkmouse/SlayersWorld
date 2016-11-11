#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "../TileSprite.hpp"
#include "../../Define.hpp"
#include "../Window.hpp"
#include "../../World/Events/Events.hpp"
#include "WritingField.hpp"
#include "HistoryField.hpp"

class InterfaceManager
{
public:
    InterfaceManager(Events*);
    ~InterfaceManager();

    void Initialize();
    void Update(sf::Time);
    TileSprite GetField(uint16, uint16);
    TileSprite GetFlask(uint8, bool, uint8 p_Pct = 100);
    TileSprite GetXpBar(bool, uint8 p_Pct = 100);
    void Draw(Window &);
    HistoryField* GetHistoryField() const;

private:
    sf::Texture     m_SystemTexture;
    sf::Texture     m_FlaskTexture;
    sf::Texture     m_XpTexture;
    Events*         m_Events;
    WritingField*   m_WritingField;
    HistoryField*   m_HistoryField;
};

