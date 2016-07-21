#pragma once
#include <vector>
#include <SFML/Window/Event.hpp>
#include "../../Define.hpp"
#include "Movement.hpp"
#include <map>

class Events
{
public:
    Events();
    ~Events();
    void NewKeyPressed(sf::Keyboard::Key);
    void KeyRelease(sf::Keyboard::Key);
    void SetActualOrientation(Orientation);
    Movement* GetMovementHandler() const;

private:
    std::map<sf::Keyboard::Key, uint8> m_DirectionMap;
    std::vector<sf::Keyboard::Key> m_KeyPressed;
    Movement* m_Movement;
    bool m_IsInMouvement;
};

