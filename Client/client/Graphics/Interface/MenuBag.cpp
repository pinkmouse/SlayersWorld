#include "MenuBag.hpp"
#include "../../Global.hpp"

SubMenuBag::SubMenuBag() :
    Menu(3, 8)
{
    m_Pos.x = 320;
    m_Pos.y = 130;
    m_ItemToDeplace = -1;
}

SubMenuBag::~SubMenuBag()
{
}


int16 SubMenuBag::GetItemToDeplace() const
{
    return m_ItemToDeplace;
}

void SubMenuBag::SetItemToDeplace(const int16 & p_SlotID)
{
    m_ItemToDeplace = p_SlotID;
}

void SubMenuBag::Open()
{
    Menu::Open();
    ClearElements();
    AddElement(0, 0, m_CurrentItem.first.m_Name);
    switch (m_CurrentItem.first.m_Type)
    {
    case eItemType::ITEM_CONSUMABLE:
        AddElement(0, 1, "Consommable");
        AddElement(0, 2, "lvl " + std::to_string(m_CurrentItem.first.m_Level));
        break;
    case eItemType::ITEM_LAUNCHSPELL:
        AddElement(0, 1, "Outils");
        AddElement(0, 2, "lvl " + std::to_string(m_CurrentItem.first.m_Level));
        break;
    case eItemType::ITEM_EQUIPMENT:
        AddElement(0, 1, "Equipement");
        AddElement(0, 2, "lvl " + std::to_string(m_CurrentItem.first.m_Level));
        switch (m_CurrentItem.first.m_SubType)
        {
        case eTypeEquipment::EQUIP_BODY:
            AddElement(0, 3, "Corps");
            break;
        case eTypeEquipment::EQUIP_FOOT:
            AddElement(0, 3, "Pieds");
            break;
        case eTypeEquipment::EQUIP_HAND:
            AddElement(0, 3, "Mains");
            break;
        case eTypeEquipment::EQUIP_HEAD:
            AddElement(0, 3, "Tête");
            break;
        case eTypeEquipment::EQUIP_SHOLDER:
            AddElement(0, 3, "Epaules");
            break;
        case eTypeEquipment::EQUIP_MAIN_HAND:
            AddElement(0, 3, "Arme Droite");
            break;
        case eTypeEquipment::EQUIP_SECOND_HAND:
            AddElement(0, 3, "Arme Gauche");
            break;
        default:
            break;
        }
        AddElement(0, 4, "Force+" + std::to_string(m_CurrentItem.first.GetData(0)));
        AddElement(0, 5, "Stamina+" + std::to_string(m_CurrentItem.first.GetData(1)));
        AddElement(0, 6, "Dexterity+" + std::to_string(m_CurrentItem.first.GetData(2)));
        switch (m_CurrentItem.first.m_SubType)
        {
        case eTypeEquipment::EQUIP_BODY:
        case eTypeEquipment::EQUIP_FOOT:
        case eTypeEquipment::EQUIP_HAND:
        case eTypeEquipment::EQUIP_HEAD:
        case eTypeEquipment::EQUIP_SHOLDER:
            AddElement(0, 7, "Armure+" + std::to_string(m_CurrentItem.first.GetData(3)));
            break;
        case eTypeEquipment::EQUIP_MAIN_HAND:
        case eTypeEquipment::EQUIP_SECOND_HAND:
            AddElement(0, 7, "Attaque+" + std::to_string(m_CurrentItem.first.GetData(3)));
            break;
        default:
            break;
        }
        break;
    case eItemType::ITEM_QUEST:
        AddElement(0, 1, "Quête");
        break;
    case eItemType::ITEM_USELESS:
        AddElement(0, 1, "Inutile");
        break;
    default:
        break;
    }

    switch (m_CurrentItem.first.m_Type)
    {
    case eItemType::ITEM_CONSUMABLE :
    case eItemType::ITEM_LAUNCHSPELL :
        AddElement(2, 0, "Utiliser");
        GetElement(2, 0)->SetFunc(&Menu::GenericAction, 0);
        break;
    case eItemType::ITEM_EQUIPMENT:
        AddElement(2, 0, "Equiper");
        GetElement(2, 0)->SetFunc(&Menu::GenericAction, 0);
        break;
    default:
        break;
    }

    if (GetElement(2, 0))
    {
        if (m_CurrentItem.first.m_Level > g_Player->GetLevel())
        {
            GetElement(2, 0)->Disable();
            SetSelectedElement(2, 1);
        }
        else
            SetSelectedElement(2, 0);
    }
    else
        SetSelectedElement(2, 1);
    AddElement(2, 1, "Déplacer");
    GetElement(2, 1)->SetFunc(&Menu::GenericAction, 2);
    AddElement(2, 2, "Suppr");
    GetElement(2, 2)->SetFunc(&Menu::GenericAction, 3);
}

void SubMenuBag::KeyPress(const sf::Keyboard::Key & p_Key)
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

void SubMenuBag::GenericAction(const uint16 & p_ActionID)
{
    switch (p_ActionID)
    {
    case 0:
        g_Socket->SendItemAction(m_CurrentItem.second);
        break;
    case 2:
        SetItemToDeplace(m_CurrentItem.second);
        break;
    case 3:
        g_Socket->SendRemoveItem(m_CurrentItem.second);
        break;
    default:
        break;

    }
    Close();
}

void SubMenuBag::SetCurrentItem(const Item  & p_Item, const uint8 & p_SlotID)
{
    m_CurrentItem.first = p_Item;
    m_CurrentItem.second = p_SlotID;
}

MenuBag::MenuBag() :
    Menu(3, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    CanSelectUselessFields();
    m_Size = 0;
    SetSubMenu(&m_SubMenu);

    m_ElementSize.first = MENU_COLUMN_SIZE * 1.5f;
    m_ElementSize.second = MENU_ROW_SIZE * 2;
}

MenuBag::~MenuBag()
{
}

void MenuBag::Open()
{
    Menu::Open();
    SetSelectedElement(0, 1);
}

Item* MenuBag::GetItem(const uint8 & p_SlotID)
{
    if (m_Items.find(p_SlotID) == m_Items.end())
        return nullptr;
    return &m_Items[p_SlotID];
}

void MenuBag::SetStackItem(const uint8 & p_SlotID, const uint8 & p_Stack)
{
    if (m_Items.find(p_SlotID) == m_Items.end())
        return;

    m_Items[p_SlotID].m_StackNb = p_Stack;
    uint8 l_RealSlot = p_SlotID + GetColumn();
    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr)
        return;
    
    GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->SetLabel(m_Items[p_SlotID].m_Name + "\nx" + std::to_string(m_Items[p_SlotID].m_StackNb));
}

void MenuBag::SetCurrency(const eTypeCurrency & p_Type, const uint16 & p_Value)
{
    m_Currencies[p_Type] = p_Value;
    switch(p_Type)
    {
        case eTypeCurrency::CURRENCY_BASISC:
        {
            uint16 l_Pc = p_Value % 100;
            uint16 l_Pa = p_Value / 100;
            uint16 l_Po = l_Pa / 100;
            l_Pa = l_Pa - (l_Po * 100);

            AddElement(0, 0, std::to_string(l_Po) + "po " + std::to_string(l_Pa) + "pa " + std::to_string(l_Pc) + "pc");
            break;
        }
        default:
            break;
    }
}

void MenuBag::AddItem(const uint8 & p_SlotID, const Item & p_Item)
{
    m_Items[p_SlotID] = p_Item;
    uint8 l_RealSlot = p_SlotID + GetColumn();
    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr || !GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->HasFunc())
    {
        AddElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn(), p_Item.m_Name + "\nx" + std::to_string(p_Item.m_StackNb));
        GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn())->SetFunc(&Menu::GenericAction, p_SlotID);
    }
}

void MenuBag::RemoveItem(const uint8 & p_SlotID)
{
    uint8 l_RealSlot = p_SlotID + GetColumn();

    if (GetElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn()) == nullptr)
        return;

    RemoveElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn());
    AddElement(l_RealSlot % GetColumn(), l_RealSlot / GetColumn(), "vide");
}

void MenuBag::GenericAction(const uint16 & p_SlotID)
{
    m_SubMenu.SetCurrentItem(m_Items[p_SlotID], p_SlotID);
    if (!m_SubMenu.IsOpen())
        m_SubMenu.Open();
}

void MenuBag::SetSize(const uint8 & p_Size)
{
    m_Size = p_Size;
    for (uint8 i = GetColumn(); i < m_Size + GetColumn(); i++)
    {
        if (GetElement(i % GetColumn(), i / GetColumn()) == nullptr)
            AddElement(i % GetColumn(), i / GetColumn(), "vide");
    }
}

void MenuBag::KeyPress(const sf::Keyboard::Key & p_Key)
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
