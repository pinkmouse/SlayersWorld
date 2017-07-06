#include "BGCapturePoint.hpp"
#include "../../../Global.hpp"

BGCapturePoint::BGCapturePoint() :
    Map::Map(g_MapManager->GetMapTemplate(1))
{
}

BGCapturePoint::~BGCapturePoint()
{
}

void BGCapturePoint::Update(sf::Time p_Time)
{
    Map::Update(p_Time);
}
