#include "SubRequiredQuest.hpp"
#include "../../Global.hpp"

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
    if (g_SqlManager->GetDaysSinceLastQuestDone(p_Player, m_QuestID) > -1)
        return true;
    return false;
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
    if (g_SqlManager->GetDaysSinceLastQuestDone(p_Player, m_QuestID) > -1)
        return false;
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

SubRequiredQuestInProgress::SubRequiredQuestInProgress(uint16 p_QuestID, uint16 p_Nb) :
    m_QuestID(p_QuestID),
    m_Number(p_Nb)
{
}

SubRequiredQuestInProgress::~SubRequiredQuestInProgress()
{
}

bool SubRequiredQuestInProgress::IsValid(const Player* p_Player) const
{
    Quest* l_Quest = p_Player->GetQuest(m_QuestID);
    if (l_Quest == nullptr)
        return false;
    return true;
}