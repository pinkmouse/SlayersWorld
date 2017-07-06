#pragma once
#include "../../../Map/Map.hpp"
#define NB_PLAYERS_CAPTURE_POINT 20

class BGCapturePoint : public Map
{
public:
    BGCapturePoint();
    ~BGCapturePoint();
    void Update(sf::Time);

private:
};