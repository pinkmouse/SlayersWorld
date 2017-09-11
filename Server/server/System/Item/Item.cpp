#include "Item.hpp"

Item::Item(Player* p_Owner, ItemTemplate* p_ItemTemplate) :
    m_Owner(p_Owner),
    m_ItemTemplate(p_ItemTemplate)
{
    m_CurrentStackNb = 1;
}

Item::~Item()
{
}

void Item::ItemAction()
{

}

bool Item::SubStack()
{
    if (m_CurrentStackNb <= 0)
        return false;

    m_CurrentStackNb--;
    return true;
}

bool Item::AddStack()
{
    if (m_ItemTemplate == nullptr)
        return false;

    if (m_ItemTemplate->m_StackNb > m_CurrentStackNb)
    {
        m_CurrentStackNb++;
        return true;
    }
    return false;
}

uint8 Item::GetStackNb() const
{
    return m_CurrentStackNb;
}

void Item::SetStackNb(const uint8 & p_StackNb)
{
    m_CurrentStackNb = p_StackNb;
}

ItemTemplate* Item::GetTemplate() const
{
    return m_ItemTemplate;
}