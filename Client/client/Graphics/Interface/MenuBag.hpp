#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

struct Item
{
    eItemType m_Type;
    std::string m_Name;
    eItemRareLevel m_RareLevel;
    uint8 m_Level;
    uint8 m_StackNb;
    std::vector<int32> m_Datas;

    Item() :
        m_Type(eItemType::ITEM_USELESS), m_Name(""), m_RareLevel(eItemRareLevel::ITEM_RARE1), m_Level(0), m_StackNb(1) {}

    Item(const eItemType & p_Type, const std::string & p_Name, const eItemRareLevel & p_RareLevel, const uint8 p_Level, const uint8 p_StackNb) :
        m_Type(p_Type), m_Name(p_Name), m_RareLevel(p_RareLevel), m_Level(p_Level), m_StackNb(p_StackNb) {}


    void AddData(const int32 & p_Data)
    {
        m_Datas.push_back(p_Data);
    }

    int32 GetData(const uint8 & p_DataNB)
    {
        if (p_DataNB >= m_Datas.size())
            return -1;
        return m_Datas[p_DataNB];
    }
};

class MenuBag : public Menu
{
public:
    MenuBag();
    ~MenuBag();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddItem(const uint8 &, const Item &);
    void RemoveItem(const uint8 &);

private:
    std::map<uint8, Item> m_Items;
};