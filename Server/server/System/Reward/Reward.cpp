#include "Reward.hpp"

Reward::Reward(const uint16 & p_ID) :
    m_ID(p_ID)
{
}

Reward::~Reward()
{
}

uint16 Reward::GetID() const
{
    return m_ID;
}

void Reward::AddSubReward(const SubReward & p_SubReward)
{
    m_ListSubReward.push_back(p_SubReward);
}

std::vector<SubReward>* Reward::GetListSubReward()
{
    return &m_ListSubReward;
}