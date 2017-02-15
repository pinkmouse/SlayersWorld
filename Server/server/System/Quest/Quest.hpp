#pragma once
#include "../../Define.hpp"

class Quest
{
public:
    Quest(QuestTemplate*);
    ~Quest();
    void SetObjectifProgress(ObjectifProgess*);
    std::vector<std::string> CheckAtEvent(eObjectifType, int32);
    bool IsDone();
    std::string GetName() const;
    uint16 GetID();

private:
    std::map< uint8, ObjectifProgess* > m_ObjectifProgressList;
    QuestTemplate* m_QuestTemplate;
};