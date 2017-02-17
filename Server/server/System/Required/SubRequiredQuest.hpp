#pragma once
#include "../../Define.hpp"
#include "../../Entities/Player.hpp"
#include "SubRequired.hpp"

class SubRequiredQuestDone : public SubRequired
{
public:
    SubRequiredQuestDone(uint16, uint16);
    ~SubRequiredQuestDone();
    bool IsValid(const Player*) const;

private:
    uint16 m_QuestID;
    uint16 m_Number;
};

class SubRequiredQuestAllObjectiveDone : public SubRequired
{
public:
    SubRequiredQuestAllObjectiveDone(uint16, uint16);
    ~SubRequiredQuestAllObjectiveDone();
    bool IsValid(const Player*) const;

private:
    uint16 m_QuestID;
    uint16 m_Number;
};

class SubRequiredQuestNotDone : public SubRequired
{
public:
    SubRequiredQuestNotDone(uint16, uint16);
    ~SubRequiredQuestNotDone();
    bool IsValid(const Player*) const;

private:
    uint16 m_QuestID;
    uint16 m_Number;
};