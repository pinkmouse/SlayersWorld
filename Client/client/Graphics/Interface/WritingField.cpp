#include "WritingField.hpp"
#include "../../Global.hpp"

WritingField::WritingField() :
    m_IsOpen(false),
    m_WritingString("")
{
    m_Text.setFont(*g_Font);
    m_Text.setCharacterSize(18);
    m_Text.setColor(sf::Color::White);
    m_Text.setPosition(5, Y_WINDOW - SIZE_FILED_TALK_Y);
    m_Prefix = "";
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
    if (!m_WritingString.empty())
        g_Socket->SendTalk(m_WritingString);

    std::string l_Cmd = m_WritingString;
    std::vector<std::string> l_CmdList;

    size_t l_Pos = 0;
    std::string l_Token;
    while ((l_Pos = l_Cmd.find(' ')) != std::string::npos)
    {
        l_Token = l_Cmd.substr(0, l_Pos);
        l_CmdList.push_back(l_Token);
        l_Cmd.erase(0, l_Pos + 1);
    }
    l_CmdList.push_back(l_Cmd.c_str());

    if (!l_CmdList.empty())
    {
        if (l_CmdList[0] == "/gr" || l_CmdList[0] == "/team")
            m_Prefix = l_CmdList[0] + " ";
        else
            m_Prefix = "";
    }
    ClearTxt();
}

void WritingField::ClearTxt()
{
    m_WritingString = m_Prefix;
    m_Text.setString(m_WritingString);
}

std::string & WritingField::AddLetter(uint32 l_Letter)
{
    if (m_WritingString.size() >= MAX_LETTER_STR)
        return m_WritingString;

    m_WritingString += l_Letter;
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