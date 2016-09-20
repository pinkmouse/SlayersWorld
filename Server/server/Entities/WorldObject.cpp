#include "WorldObject.hpp"


WorldObject::WorldObject()
{
}


WorldObject::~WorldObject()
{
}

float WorldObject::GetDistance(const Position & p_Position) const
{
    uint32 l_X = std::max(GetPosX(), p_Position.x) - std::min(GetPosX(), p_Position.x);
    uint32 l_Y = std::max(GetPosY(), p_Position.y) - std::min(GetPosY(), p_Position.y);

    float l_Dist = sqrt((float)((l_X * l_X) + (l_Y * l_Y)));
    return l_Dist;
}

float WorldObject::GetDistance(const WorldObject* p_Object) const
{
    return GetDistance(p_Object->GetPosition());
}