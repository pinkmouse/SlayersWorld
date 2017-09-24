#include "MenuSell.hpp"
#include "../../Global.hpp"
#include "../../System/CurrencyUtils.hpp"

SubMenuSell::SubMenuSell() :
    Menu(3, 2)
{
    m_Pos.x = 320;
    m_Pos.y = 130;
}

SubMenuSell::~SubMenuSell()
{
}

void SubMenuSell::Open()
{
    Menu::Open();
    ClearElements();
    AddElement(0, 0, m_CurrentItem.first.m_Name);
    AddElement(0, 1, CurrencyToString(ConvertPcToBasicCurrency(m_CurrentItemPrice)));

    AddElement(2, 0, "Vendre");
    GetElement(2, 0)->SetFunc(&Menu::GenericAction, 0);

    SetSelectedElement(2, 0);
}

void SubMenuSell::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
    case sf::Keyboard::Escape:
    {
        if (IsOpen())
            Close();
        break;
    }
    case sf::Keyboard::Space:
    {
        std::pair<uint8, uint8> l_SelectedElem = GetSelectedElement();
        MenuElement* l_Elem = GetElement(l_SelectedElem.first, l_SelectedElem.second);
        if (l_Elem == nullptr)
            break;
        l_Elem->LaunchFunc(l_Elem->GetIDLabel(), this);
        break;
    }
    case sf::Keyboard::Up:
    {
        SelectNextElementOn(Orientation::Up);
        break;
    }
    case sf::Keyboard::Down:
    {
        SelectNextElementOn(Orientation::Down);
        break;
    }
    case sf::Keyboard::Left:
    {
        SelectNextElementOn(Orientation::Left);
        break;
    }
    case sf::Keyboard::Right:
    {
        SelectNextElementOn(Orientation::Right);
        break;
    }
    }
}

void SubMenuSell::GenericAction(const uint16 & p_ActionID)
{
    switch (p_ActionID)
    {
    case 0:
        g_Socket->SendItemSell(m_CurrentItem.second);
        break;
    default:
        break;
    }
    Close();
}

void SubMenuSell::SetCurrentItem(const Item  & p_Item, const uint8 & p_SlotID)
{
    m_CurrentItem.first = p_Item;
    m_CurrentItem.second = p_SlotID;
}

void SubMenuSell::SetCurrentItemPrice(const uint16 & p_Price)
{
    m_CurrentItemPrice = p_Price;
}


MenuSell::MenuSell() :
    Menu(4, 7)
{
    m_Pos.x = 20;
    m_Pos.y = 20;
    CanSelectUselessFields();
    m_Size = 0;
    SetSubMenu(&m_SubMenu);

    m_ElementSize.first = MENU_COLUMN_SIZE * 1.5f;
    m_ElementSize.second = MENU_ROW_SIZE * 3;
}

MenuSell::~MenuSell()
{
}

void MenuSell::Open()
{
    Menu::Open();
    SetSelectedElement(0, 1);
}

Item* MenuSell::GetItem(const uint8 & p_SlotID)
{
    if (m_Items.find(p_SlotID) == m_Items.end())
        return nullptr;
    return &m_Items[p_SlotID];
}

void MenuSell::SetStackItem(const uint8 & p_SlotID, const uint8 & p_Stack)
{
    if (m_Items.find(p_SlotID) == m_Items.end())
        return;

    m_Items[p_SlotID].m_StackNb = p_Stack;
    uint8 l_RealSlot = p_SlotID + GetColumn();
    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr)
        return;
    
    GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->SetLabel(m_Items[p_SlotID].m_Name + "\nx" + std::to_string(m_Items[p_SlotID].m_StackNb));
}

void MenuSell::SetCurrency(const eTypeCurrency & p_Type, const uint16 & p_Value)
{
    m_Currencies[p_Type] = p_Value;
    switch(p_Type)
    {
        case eTypeCurrency::CURRENCY_BASISC:
        {
            CurrencyBasicConvert l_Currency = ConvertPcToBasicCurrency(p_Value);
        
            AddElement(0, 0, CurrencyToString(l_Currency));
            break;
        }
        default:
            break;
    }
}

void MenuSell::AddItem(const uint8 & p_SlotID, const Item & p_Item)
{
    m_Items[p_SlotID] = p_Item;
    uint8 l_RealSlot = p_SlotID + GetColumn();
    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr || !GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->HasFunc())
    {
        AddElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn(), p_Item.m_Name + "\nx" + std::to_string(p_Item.m_StackNb));
        GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->SetFunc(&Menu::GenericAction, p_SlotID);
    }
}

void MenuSell::AddItemPrice(const uint8 & p_SlotID, const uint16 & p_Price)
{
    std::map<uint8, Item>::iterator l_It = m_Items.find(p_SlotID);
    if (l_It == m_Items.end())
        return;

    m_ItemsPrice[p_SlotID] = p_Price;

    uint8 l_RealSlot = p_SlotID + GetColumn();
    CurrencyBasicConvert l_Currency = ConvertPcToBasicCurrency(p_Price);
    MenuElement* l_Element = GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn());

    if (l_Element == nullptr)
        return;

    l_Element->SetLabel((*l_It).second.m_Name + "\nx" + std::to_string((*l_It).second.m_StackNb) + "\n" + CurrencyToString(l_Currency));
    l_Element->SetFunc(&Menu::GenericAction, p_SlotID);
}

void MenuSell::RemoveItem(const uint8 & p_SlotID)
{
    uint8 l_RealSlot = p_SlotID + GetColumn();

    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr)
        return;

    RemoveElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn());
    AddElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn(), "vide");
}

void MenuSell::GenericAction(const uint16 & p_SlotID)
{
    m_SubMenu.SetCurrentItem(m_Items[p_SlotID], p_SlotID);
    if (m_ItemsPrice.find(p_SlotID) != m_ItemsPrice.end())
        m_SubMenu.SetCurrentItemPrice(m_ItemsPrice[p_SlotID]);
    else
        m_SubMenu.SetCurrentItemPrice(0);

    if (!m_SubMenu.IsOpen())
        m_SubMenu.Open();
}

void MenuSell::SetSize(const uint8 & p_Size)
{
    m_Size = p_Size;
    for (uint8 i = GetColumn(); i < m_Size + GetColumn(); i++)
    {
        if (GetElement(i % GetColumn(), i / GetColumn()) == nullptr)
            AddElement(i % GetColumn(), i / GetColumn(), "vide");
    }
}

void MenuSell::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
        case sf::Keyboard::Escape:
        {
            if (IsOpen())
                Close();
            break;
        }
        case sf::Keyboard::Space:
        {
            std::pair<uint8, uint8> l_SelectedElem = GetSelectedElement();
            MenuElement* l_Elem = GetElement(l_SelectedElem.first, l_SelectedElem.second);
            if (l_Elem == nullptr)
                break;
            l_Elem->LaunchFunc(l_Elem->GetIDLabel(), this);
            break;
        }
        case sf::Keyboard::Up:
        {
            SelectNextElementOn(Orientation::Up);
            break;
        }
        case sf::Keyboard::Down:
        {
            SelectNextElementOn(Orientation::Down);
            break;
        }
        case sf::Keyboard::Left:
        {
            SelectNextElementOn(Orientation::Left);
            break;
        }
        case sf::Keyboard::Right:
        {
            SelectNextElementOn(Orientation::Right);
            break;
        }
    }
}
