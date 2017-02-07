#pragma once
#include "../../Define.hpp"

class Quest
{
public:
    Quest(QuestTemplate*);
    ~Quest();
    void SetObjectifProgress(ObjectifProgess*);
    void CheckAtEvent(eObjectifType, int32);
    bool IsDone();
    uint16 GetID();

private:
    std::map< uint8, ObjectifProgess* > m_ObjectifProgressList;
    QuestTemplate* m_QuestTemplate;
};