#pragma once
#include "../TileSprite.hpp"
#include "../../Define.hpp"
#include <string>
#include <SFML/Graphics/Text.hpp>

class WritingField
{
public:
    WritingField();
    ~WritingField();
    bool IsFieldOpen();
    void Open();
    void Close();
    void ClearTxt();
    std::string & AddLetter(uint32);
    void RemoveLastLetter();
    const std::string & GetString() const;
    sf::Text GetText();

private:
    bool m_IsOpen;
    sf::Text m_Text;
    std::string m_WritingString;
};