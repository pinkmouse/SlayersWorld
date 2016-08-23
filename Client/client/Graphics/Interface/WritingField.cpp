#include "WritingField.hpp"
#include "../../Global.hpp"

WritingField::WritingField() :
    m_IsOpen(false),
    m_WritingString("")
{
    m_Text.setFont(*g_Font);
    m_Text.setCharacterSize(20);
    m_Text.setColor(sf::Color::White);
    m_Text.setPosition(5, Y_WINDOW - SIZE_FILED_TALK_Y + 3);
}


WritingField::~WritingField()
{
}

void WritingField::Open()
{
    m_IsOpen = true;
}

void WritingField::Close()
{
    m_IsOpen = false;
    ClearTxt();
}

void WritingField::ClearTxt()
{
    m_WritingString = "";
    m_Text.setString(m_WritingString);
}

std::string & WritingField::AddLetter(uint32 l_Letter)
{
    m_WritingString += l_Letter;
    printf("%s\n", m_WritingString.c_str());
    m_Text.setFont(*g_Font);
    m_Text.setString(m_WritingString);
    return m_WritingString;
}

const std::string & WritingField::GetString() const
{
    return m_WritingString;
}

sf::Text WritingField::GetText()
{
    return m_Text;
}

bool WritingField::IsFieldOpen()
{
    return m_IsOpen;
}

void WritingField::RemoveLastLetter()
{
    if (m_WritingString.empty())
        return;

    m_WritingString.resize(m_WritingString.size() - 1);
    m_Text.setFont(*g_Font);
    m_Text.setString(m_WritingString);
}