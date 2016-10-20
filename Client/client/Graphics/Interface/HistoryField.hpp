#pragma once
#include "../TileSprite.hpp"
#include "../../Define.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics/Text.hpp>

#define MAX_HISTORY_LINE 5

class HistoryField
{
public:
    HistoryField();
    ~HistoryField();
    void Open();
    void Close();
    bool IsFieldOpen();
    void AddHistoryLine(const std::string &);
    sf::Text GetText();
    std::vector<std::string> GetHistory() const;

private:
    sf::Text m_Text;
    bool m_IsOpen;
    std::vector<std::string> m_History;
};