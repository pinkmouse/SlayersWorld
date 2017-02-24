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
    void InitializeWarningMsgs();
    void Update(sf::Time);
    void ManageEvent(sf::Event);
    TileSprite GetField(uint16, uint16);
    TileSprite GetBorderField(uint16, uint16);
    TileSprite GetFlask(uint8, bool, uint8 p_Pct = 100);
    TileSprite GetXpBar(bool, uint8 p_Pct = 100);
    TileSprite GetCastBar(bool, uint8 p_Pct = 100);
    void Draw(Window &);
    void DrawStartingPage(Window &);
    void DrawWarnings(Window &);
    void DrawClock(Window &);
    HistoryField* GetHistoryField() const;
    void AddWarningMsg(eTypeWarningMsg, const std::string &);
    void AddWarningMsg(eTypeWarningMsg, eWarningMsg);
    void AddKeyBind(uint8, uint8);
    int16 GetBindForKey(uint8);
    void SetSystemMsg(const std::string &);
    void AddBlockingBind(uint8, uint16);
    bool IsBlockingBind(uint8);
    sf::Vector2i TextSplitToFit(uint16, sf::Text &);

private:
    sf::Texture                         m_SystemTexture;
    sf::Texture                         m_FlaskTexture;
    sf::Texture                         m_XpTexture;
    sf::Texture                         m_CastBarTexture;
    sf::Texture                         m_Background;
    Events*                             m_Events;
    WritingField*                       m_WritingField;
    HistoryField*                       m_HistoryField;
    sf::Text                            m_SystemMsg;
    sf::Text                            m_ClockTxt;
    std::map< eTypeWarningMsg, std::vector< std::pair<std::string, uint32> > >     m_WarningMsgs;
    std::map< eWarningMsg, std::string > m_WarningMsgsEnum;
    std::map< uint8, uint8 >             m_KeyBoardBind;
    std::map< uint8, uint64 >            m_BlockingBinds;
};