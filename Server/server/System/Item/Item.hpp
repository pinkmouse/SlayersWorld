#pragma once
#include "../../Define.hpp"

class Player;

struct ItemTemplate
{
    uint16 m_Id;
    eItemType m_Type;
    std::string m_Name;
    uint8 m_StackNb;
    eItemRareLevel m_RareLevel;
    Required* m_Required;
    std::vector<int32> m_Data;
    uint8 m_Level;

    ItemTemplate() :
        m_Id(0), m_Type(eItemType::ITEM_USELESS), m_Name(""), m_Level(0), m_StackNb(0), m_RareLevel(eItemRareLevel::ITEM_RARE1), m_Required(nullptr) {}

    ItemTemplate(const uint16 & p_Id, const eItemType & p_Type, const std::string & p_Name, const uint8 & p_Level, const uint8 & p_StackNB, const eItemRareLevel & p_RareLevel, Required* p_Required) :
        m_Id(p_Id), m_Type(p_Type), m_Name(p_Name), m_Level(p_Level),  m_StackNb(p_StackNB), m_RareLevel(p_RareLevel), m_Required(p_Required) {}

    void AddData(const int32 & p_Data)
    {
        m_Data.push_back(p_Data);
    }

    int32 GetData(const uint8 & p_DataNB)
    {
        if (p_DataNB >= m_Data.size())
            return -1;
        return m_Data[p_DataNB];
    }
};

class Item
{
public:
    Item(Player*, ItemTemplate*);
    ~Item();
    void ItemAction();
    bool AddStack();
    bool SubStack();
    uint8 GetStackNb() const;
    void SetStackNb(const uint8 &);
    ItemTemplate* GetTemplate() const;

private:
    ItemTemplate* m_ItemTemplate;
    Player* m_Owner;
    uint8 m_CurrentStackNb;
};