#pragma once
#include <vector>
#include <SFML/Window/Event.hpp>
#include "../../Define.hpp"
#include "../../Graphics/Interface/WritingField.hpp"
#include <map>
#include <string>

class Events
{
public:
    Events();
    ~Events();
    void Update();
    void NewKeyPressed(sf::Keyboard::Key);
    void KeyRelease(sf::Keyboard::Key);
    void TextEntered(sf::Uint32);
    void SetWritingField(WritingField*);

private:
    std::map<sf::Keyboard::Key, uint8> m_DirectionMap;
    std::vector<sf::Keyboard::Key> m_KeyPressed; ///< Only for Direction
    bool m_IsInMouvement;

    WritingField* m_WritingField;
};

