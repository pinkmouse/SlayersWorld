#include "Quest.hpp"

Quest::Quest(QuestTemplate* p_QuestTemplate) :
    m_QuestTemplate(p_QuestTemplate)
{
        for (std::map< uint8, ObjectifQuestTemplate* >::iterator l_It = p_QuestTemplate->m_ObjectifList.begin(); l_It != p_QuestTemplate->m_ObjectifList.end(); ++l_It)
        m_ObjectifProgressList[(*l_It).first] = new ObjectifProgess((*l_It).first);
}

Quest::~Quest()
{
}

uint16 Quest::GetID() const
{
    return m_QuestTemplate->m_Id;
}

void Quest::SetObjectifProgress(ObjectifProgess* p_ObjectifProgress)
{
    if (m_ObjectifProgressList.find(p_ObjectifProgress->m_Id) != m_ObjectifProgressList.end())
        delete m_ObjectifProgressList[p_ObjectifProgress->m_Id];
    m_ObjectifProgressList[p_ObjectifProgress->m_Id] = p_ObjectifProgress;
}

QuestTemplate* Quest::GetQuestTemplate() const
{
    return m_QuestTemplate;
}

void Quest::SetDataOfObjectif(uint8 p_ObjectifID,  uint8 p_DataID, int16 p_Data)
{
    if (m_ObjectifProgressList.find(p_ObjectifID) == m_ObjectifProgressList.end())
        return;

    /* Actually there is only Data0 */
    switch (p_DataID)
    {
    case 0:
        m_ObjectifProgressList[p_ObjectifID]->m_Data0 = p_Data;
        break;
    }
}

std::map< uint8, ObjectifProgess* >* Quest::GetObjectifsProgress()
{
    return &m_ObjectifProgressList;
}


std::vector<std::string> Quest::CheckAtEvent(eObjectifType p_ObjectifType, int32 p_Data0)
{
    std::vector<std::string> l_List;
    for (std::map< uint8, ObjectifQuestTemplate* >::iterator l_It = m_QuestTemplate->m_ObjectifList.begin(); l_It != m_QuestTemplate->m_ObjectifList.end(); ++l_It)
    {
        if ((*l_It).second->m_ObjectifType != p_ObjectifType)
            continue;

        if ((*l_It).second->m_Data0 != p_Data0)
            continue;

        if (m_ObjectifProgressList[(*l_It).first]->m_Data0 >= (*l_It).second->m_Data1)
            continue;

        /// Pct chance on KillMob
        if (p_ObjectifType == eObjectifType::KillMob && (*l_It).second->m_Data2 && !RandChance((uint8)(*l_It).second->m_Data2))
            continue;

        m_ObjectifProgressList[(*l_It).first]->m_Data0 += 1; /// TODO
        l_List.push_back((*l_It).second->m_Entitled + " " + std::to_string(m_ObjectifProgressList[(*l_It).first]->m_Data0) + "/" + std::to_string((*l_It).second->m_Data1));
    }
    return l_List;
}

bool Quest::IsDone()
{
    for (std::map< uint8, ObjectifQuestTemplate* >::iterator l_It = m_QuestTemplate->m_ObjectifList.begin(); l_It != m_QuestTemplate->m_ObjectifList.end(); ++l_It)
    {
        if (m_ObjectifProgressList[(*l_It).first]->m_Data0 < (*l_It).second->m_Data1)
            return false;
    }
    printf("Quest Done\n");
    return true;
}

std::string Quest::GetName() const
{
    return m_QuestTemplate->m_Name;
}