#include "BGCapturePoint.hpp"


BGCapturePoint::BGCapturePoint(std::vector<Player*> p_ListPlayer) :
    BG(TypeBG::PointCapture, 1)
{

    for (uint8 i = 0; i < p_ListPlayer.size(); i++)
    {
        //p_ListPlayer[i]->TeleportTo()
    }
}

BGCapturePoint::~BGCapturePoint()
{
}

void BGCapturePoint::Update(sf::Time p_Time)
{
    BG::Update(p_Time);
}
