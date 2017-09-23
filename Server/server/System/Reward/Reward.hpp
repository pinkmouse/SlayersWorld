#pragma once
#include "../../Define.hpp"

struct SubReward
{
    Required* m_Required;
    eRewardType m_Type;
    std::vector<int32> m_Data;
    float m_ChancePct;

    SubReward(const eRewardType & p_Type, Required* p_Required, const float & p_ChancePct) :
        m_Type(p_Type),
        m_Required(p_Required),
        m_ChancePct(p_ChancePct) {}

    void AddData(const int32 & p_Data)
    {
        m_Data.push_back(p_Data);
    }

    uint16 GetData(const uint8 & p_Data)
    {
        return m_Data[p_Data];
    }
};

class Reward
{
public:
    Reward(const uint16 &);
    ~Reward();
    uint16 GetID() const;
    void AddSubReward(const SubReward &);
    std::vector<SubReward>* GetListSubReward();

private:
    uint16 m_ID;
    std::vector<SubReward> m_ListSubReward;
};