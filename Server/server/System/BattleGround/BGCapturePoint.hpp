#pragma once
#include "BG.hpp"

#define NB_PLAYERS_CAPTURE_POINT 20

class BGCapturePoint : public BG
{
public:
    BGCapturePoint(std::vector<Player*>);
    ~BGCapturePoint();
    void Update(sf::Time);

private:
};