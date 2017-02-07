#include "QuestManager.hpp"

QuestManager::QuestManager()
{
}

QuestManager::~QuestManager()
{
}

void QuestManager::AddQuestTemplate(QuestTemplate* p_QuestTemplate)
{
    if (m_QuestTemplateList.find(p_QuestTemplate->m_Id) != m_QuestTemplateList.end())
        delete m_QuestTemplateList[p_QuestTemplate->m_Id];
    m_QuestTemplateList[p_QuestTemplate->m_Id] = p_QuestTemplate;
}

void QuestManager::AddObjectifToTemplate(uint16 p_Id, ObjectifQuestTemplate* p_Objectif)
{
    if (m_QuestTemplateList.find(p_Id) == m_QuestTemplateList.end())
        return;
    m_QuestTemplateList[p_Id]->AddObjectif(p_Objectif);
}

QuestTemplate* QuestManager::GetQuestTemplate(uint16 p_Id)
{
    if (m_QuestTemplateList.find(p_Id) == m_QuestTemplateList.end())
        return nullptr;
    return m_QuestTemplateList[p_Id];
}