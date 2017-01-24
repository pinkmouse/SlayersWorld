#include "SubRequiredQuest.hpp"

SubRequiredQuestDone::SubRequiredQuestDone(uint16 p_QuestID, uint16 p_Nb) :
    m_QuestID(p_QuestID),
    m_Number(p_Nb)
{
}

SubRequiredQuestDone::~SubRequiredQuestDone()
{
}

bool SubRequiredQuestDone::IsValid(const Player* p_Player) const
{
    return true;
}

SubRequiredQuestNotDone::SubRequiredQuestNotDone(uint16 p_QuestID, uint16 p_Nb) :
    m_QuestID(p_QuestID),
    m_Number(p_Nb) 
{
}

SubRequiredQuestNotDone::~SubRequiredQuestNotDone()
{
}

bool SubRequiredQuestNotDone::IsValid(const Player* p_Player) const
{
    return true;
}