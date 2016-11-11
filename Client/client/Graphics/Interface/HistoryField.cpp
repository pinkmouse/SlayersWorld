#include "HistoryField.hpp"
#include "../../Global.hpp"

HistoryField::HistoryField() :
    m_IsOpen(false)
{
    m_Text.setFont(*g_Font);
    m_Text.setCharacterSize(18);
    m_Text.setColor(sf::Color::White);
    m_HistoryMaxLine = MIN_HISTORY_LINE;
    m_DiffTimeOpen = 0;
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
    m_DiffTimeOpen = 0;
}

std::vector<std::string> HistoryField::GetHistory() const
{
    return m_History;
}

void HistoryField::Update(sf::Time p_Diff)
{
    if (m_DiffTimeOpen <= 0)
        return;

    if (m_DiffTimeOpen - p_Diff.asMicroseconds() > 0)
        m_DiffTimeOpen -= p_Diff.asMicroseconds();
    else
        m_DiffTimeOpen = 0;
}

void HistoryField::OpenTemporary(uint32 p_Time)
{
    if (m_IsOpen && m_DiffTimeOpen <= 0)
        return;

    m_DiffTimeOpen += p_Time * IN_MILLISECOND;
}

bool HistoryField::IsFieldOpen()
{
    return m_IsOpen || m_DiffTimeOpen > 0;
}

sf::Text HistoryField::GetText() const
{
    return m_Text;
}

void HistoryField::SetLineHistory(uint8 p_HistoryMaxLineineNb)
{
    m_HistoryMaxLine = p_HistoryMaxLineineNb;
}

uint8 HistoryField::GetLineHistory() const
{
    return m_HistoryMaxLine;
}

void HistoryField::AddHistoryLine(const std::string & p_String)
{
    m_History.push_back(p_String);
    if (m_History.size() > MAX_HISTORY_LINE)
        m_History.erase(m_History.begin());
}