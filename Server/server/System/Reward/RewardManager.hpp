#pragma once
#include "../../Define.hpp"
#include "Reward.hpp"

class RewardManager
{
public:
    RewardManager();
    ~RewardManager();
    void AddReward(Reward*);
    Reward* GetReward(const uint16 &);
    std::vector<SubReward*> GetRewardsForID(const uint16 &);

private:
    std::map< uint16, Reward* > m_RewardList;
};