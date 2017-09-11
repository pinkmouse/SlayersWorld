#pragma once
#include "../../Define.hpp"
#include "Item.hpp"

class ItemManager
{
public:
    ItemManager();
    ~ItemManager();
    void AddItemTemplate(ItemTemplate*);
    ItemTemplate* GetItemTemplate(uint16);

private:
    std::map< uint16, ItemTemplate* > m_ItemTemplateList;
};