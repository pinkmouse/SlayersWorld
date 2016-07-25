#include "ClockHandler.hpp"



ClockHandler::ClockHandler()
{
    m_PinTime = getElapsedTime();
}


ClockHandler::~ClockHandler()
{
}

void ClockHandler::PinTime()
{
    m_PinTime = getElapsedTime();
}

sf::Time ClockHandler::GetDiffPinTime()
{
    return getElapsedTime() - m_PinTime;
}
