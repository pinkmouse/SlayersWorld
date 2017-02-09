#pragma once
#include <vector>
#include <SFML/Window/Event.hpp>
#include "../../Define.hpp"
#include "../../Graphics/Interface/WritingField.hpp"
#include "../../Graphics/Interface/HistoryField.hpp"
#include <map>
#include <string>

class Events
{
public:
    Events();
    ~Events();
    void Update();
    bool IsKeyUsableWhileDeath(eKeyBoardAction);
    void NewKeyPressed(eKeyBoardAction);
    void KeyRelease(eKeyBoardAction);
    void TextEntered(sf::Uint32);
    void SetWritingField(WritingField*);
    void SetHistoryField(HistoryField*);
    void LostFocus();

private:
    std::map<eKeyBoardAction, uint8> m_DirectionMap;
    std::vector<eKeyBoardAction> m_KeyPressed; ///< Only for Direction
    std::map<eKeyBoardAction, bool> m_KeyUsableWhileDeath;
    bool m_IsInMouvement;

    WritingField* m_WritingField;
    HistoryField* m_HistoryField;
};

