#pragma once
#include "../Define.hpp"
#include <SFML/System/Clock.hpp>

class ClockHandler : public sf::Clock
{
public:
    ClockHandler();
    ~ClockHandler();
    void PinTime();
    sf::Time GetDiffPinTime();

private:
    sf::Time m_PinTime;
};

