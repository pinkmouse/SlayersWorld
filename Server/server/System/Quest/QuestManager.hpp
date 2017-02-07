#pragma once
#include "../../Define.hpp"
#include "QuestManager.hpp"

class QuestManager
{
public:
    QuestManager();
    ~QuestManager();
    void AddObjectifToTemplate(uint16, ObjectifQuestTemplate*);
    void AddQuestTemplate(QuestTemplate*);
    QuestTemplate* GetQuestTemplate(uint16);

private:
    std::map< uint16, QuestTemplate* > m_QuestTemplateList;
};