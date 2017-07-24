#include "RequiredManager.hpp"
#include "SubRequiredLevel.hpp"
#include "SubRequiredQuest.hpp"

RequiredManager::RequiredManager()
{
}

RequiredManager::~RequiredManager()
{
}

void RequiredManager::AddSubRequiered(uint16 p_RequierdID, SubRequired* p_SubRequierd)
{
    m_RequiredList[p_RequierdID].AddSubRequired(p_SubRequierd);
}

void RequiredManager::AddSubRequiered(uint16 p_RequierdID, eRequiredType p_Type, uint32 p_Data0, uint32 p_Data1)
{
    switch (p_Type)
    {
    case QuestDone:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredQuestDone((uint16)p_Data0, (uint16)p_Data1));
        break;
    case QuestNotDone:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredQuestNotDone((uint16)p_Data0, (uint16)p_Data1));
        break;
    case LevelMin:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredLevelMin((uint8)p_Data0));
        break;
    case LevelMax:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredLevelMax((uint8)p_Data0));
        break;
    case QuestAllObjectiveDone:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredQuestAllObjectiveDone((uint16)p_Data0, (uint16)p_Data1));
        break;
    case QuestInProgress:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredQuestInProgress((uint16)p_Data0, (uint16)p_Data1));
        break;
    case AccessLevel:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredAccessLevel((uint16)p_Data0, (uint16)p_Data1));
        break;
    case ClassNeed:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredClass((uint8)p_Data0));
        break;
    case HasSpell:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredHasSpell((uint16)p_Data0));
        break;
    case HasNotSpell:
        m_RequiredList[p_RequierdID].AddSubRequired(new SubRequiredHasNotSpell((uint16)p_Data0));
        break;
    default:
        break;
    }
}

Required* RequiredManager::GetRequiered(uint16 p_RequierdID)
{
    if (m_RequiredList.find(p_RequierdID) == m_RequiredList.end())
        return nullptr;
    
    return &m_RequiredList[p_RequierdID];
}