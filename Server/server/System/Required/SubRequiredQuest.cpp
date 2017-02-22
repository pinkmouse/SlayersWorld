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

SubRequiredQuestAllObjectiveDone::SubRequiredQuestAllObjectiveDone(uint16 p_QuestID, uint16 p_Nb) :
    m_QuestID(p_QuestID),
    m_Number(p_Nb)
{
}

SubRequiredQuestAllObjectiveDone::~SubRequiredQuestAllObjectiveDone()
{
}

bool SubRequiredQuestAllObjectiveDone::IsValid(const Player* p_Player) const
{
    Quest* l_Quest = p_Player->GetQuest(m_QuestID);
    if (l_Quest == nullptr)
        return false;
    if (l_Quest->IsDone())
        return true;
    return false;
}