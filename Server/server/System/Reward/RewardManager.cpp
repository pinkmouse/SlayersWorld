#include "RewardManager.hpp"

RewardManager::RewardManager()
{
}

RewardManager::~RewardManager()
{
}

void RewardManager::AddReward(Reward * p_Reward)
{
    m_RewardList[p_Reward->GetID()] = p_Reward;
}

Reward* RewardManager::GetReward(const uint16 & m_ID)
{
    if (m_RewardList.find(m_ID) == m_RewardList.end())
        return nullptr;

    return m_RewardList[m_ID];
}

std::vector<SubReward*> RewardManager::GetRewardsForID(const uint16 & m_ID)
{
    std::vector<SubReward*> l_ListRewards;

    if (m_RewardList.find(m_ID) == m_RewardList.end())
        return l_ListRewards;

    Reward* l_Reward = GetReward(m_ID);
    std::vector<SubReward>* l_List = l_Reward->GetListSubReward();

    for (uint8 i = 0; i < l_List->size(); i++)
    {
        if (RandChance(l_List->at(i).m_ChancePct))
            l_ListRewards.push_back(&l_List->at(i));
    }
    return l_ListRewards;
}