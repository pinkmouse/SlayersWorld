#pragma once
#include "../../Define.hpp"

class Quest
{
public:
    Quest(QuestTemplate*);
    ~Quest();
    void SetObjectifProgress(ObjectifProgess*);
    void SetDataOfObjectif(uint8, uint8, int16);
    std::map< uint8, ObjectifProgess* >* GetObjectifsProgress();
    std::vector<std::string> CheckAtEvent(eObjectifType, int32);
    bool IsDone();
    std::string GetName() const;
    uint16 GetID() const;
    QuestTemplate* GetQuestTemplate() const;

private:
    std::map< uint8, ObjectifProgess* > m_ObjectifProgressList;
    QuestTemplate* m_QuestTemplate;
};