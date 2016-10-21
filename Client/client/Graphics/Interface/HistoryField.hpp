#pragma once
#include "../TileSprite.hpp"
#include "../../Define.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics/Text.hpp>

class HistoryField
{
public:
    HistoryField();
    ~HistoryField();
    void Open();
    void Close();
    bool IsFieldOpen();
    void AddHistoryLine(const std::string &);
    sf::Text GetText() const;
    std::vector<std::string> GetHistory() const;
    uint8 GetLineHistory() const;
    void SetLineHistory(uint8);

private:
    sf::Text m_Text;
    bool m_IsOpen;
    std::vector<std::string> m_History;
    uint8 m_HistoryMaxLine;
};