#pragma once
#include <vector>
#include <SFML/Window/Event.hpp>
#include "../../Define.hpp"
#include <map>

class Events
{
public:
    Events();
    ~Events();
    void NewKeyPressed(sf::Keyboard::Key);
    void KeyRelease(sf::Keyboard::Key);

private:
    std::map<sf::Keyboard::Key, uint8> m_DirectionMap;
    std::vector<sf::Keyboard::Key> m_KeyPressed;
    bool m_IsInMouvement;
};

