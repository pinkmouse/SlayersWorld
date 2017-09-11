#include "ItemManager.hpp"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::AddItemTemplate(ItemTemplate* p_ItemTemplate)
{
    if (m_ItemTemplateList.find(p_ItemTemplate->m_Id) != m_ItemTemplateList.end())
        delete m_ItemTemplateList[p_ItemTemplate->m_Id];
    m_ItemTemplateList[p_ItemTemplate->m_Id] = p_ItemTemplate;
}

ItemTemplate* ItemManager::GetItemTemplate(uint16 p_Id)
{
    if (m_ItemTemplateList.find(p_Id) == m_ItemTemplateList.end())
        return nullptr;
    return m_ItemTemplateList[p_Id];
}