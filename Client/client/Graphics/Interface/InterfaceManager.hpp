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
    TileSprite GetBorderField(uint16, uint16);
    TileSprite GetFlask(uint8, bool, uint8 p_Pct = 100);
    TileSprite GetXpBar(bool, uint8 p_Pct = 100);
    void Draw(Window &);
    void DrawStartingPage(Window &);
    void DrawWarnings(Window &);
    HistoryField* GetHistoryField() const;
    void AddWarningMsg(const std::string &);
    void SetSystemMsg(const std::string &);

private:
    sf::Texture                         m_SystemTexture;
    sf::Texture                         m_FlaskTexture;
    sf::Texture                         m_XpTexture;
    sf::Texture                         m_Background;
    Events*                             m_Events;
    WritingField*                       m_WritingField;
    HistoryField*                       m_HistoryField;
    sf::Text                            m_SystemMsg;
    std::vector< std::pair<std::string, uint32> >     m_WarningMsgs;
};