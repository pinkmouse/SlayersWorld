#include "HistoryField.hpp"
#include "../../Global.hpp"

HistoryField::HistoryField() :
    m_IsOpen(false)
{
    m_Text.setFont(*g_Font);
    m_Text.setCharacterSize(18);
    m_Text.setColor(sf::Color::White);
    m_Text.setPosition(5, Y_WINDOW - SIZE_FILED_TALK_Y);
}


HistoryField::~HistoryField()
{
}

void HistoryField::Open()
{
    m_IsOpen = true;
}

void HistoryField::Close()
{
    m_IsOpen = false;
}

std::vector<std::string> HistoryField::GetHistory() const
{
    return m_History;
}

bool HistoryField::IsFieldOpen()
{
    return m_IsOpen;
}

sf::Text HistoryField::GetText()
{
    return m_Text;
}

void HistoryField::AddHistoryLine(const std::string & p_String)
{
    m_History.push_back(p_String);
    if (m_History.size() > MAX_HISTORY_LINE)
        m_History.erase(m_History.begin());
}