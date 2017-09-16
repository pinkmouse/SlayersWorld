#include "MenuEquipment.hpp"
#include "../../Global.hpp"


SubMenuEquipment::SubMenuEquipment() :
    Menu(3, 8)
{
    m_Pos.x = 320;
    m_Pos.y = 130;
}

SubMenuEquipment::~SubMenuEquipment()
{
}

void SubMenuEquipment::Open()
{
    Menu::Open();
    ClearElements();

    AddElement(2, 1, "Retirer");
    GetElement(2, 1)->SetFunc(&Menu::GenericAction, 0);
    SetSelectedElement(2, 1);
    AddElement(0, 0, m_CurrentItem.first.m_Name);
    AddElement(0, 1, "lvl " + std::to_string(m_CurrentItem.first.m_Level));

    switch ((eTypeEquipment)m_CurrentItem.first.m_SubType)
    {
    case eTypeEquipment::EQUIP_BODY:
        AddElement(0, 2, "Corps");
        break;
    case eTypeEquipment::EQUIP_FOOT:
        AddElement(0, 2, "Pieds");
        break;
    case eTypeEquipment::EQUIP_HAND:
        AddElement(0, 2, "Mains");
        break;
    case eTypeEquipment::EQUIP_HEAD:
        AddElement(0, 2, "Tête");
        break;
    case eTypeEquipment::EQUIP_SHOLDER:
        AddElement(0, 2, "Epaules");
        break;
    case eTypeEquipment::EQUIP_MAIN_HAND:
        AddElement(0, 2, "Arme Droite");
        break;
    case eTypeEquipment::EQUIP_SECOND_HAND:
        AddElement(0, 2, "Arme Gauche");
        break;
    default:
        break;
    }
    AddElement(0, 3, "Force+" + std::to_string(m_CurrentItem.first.GetData(0)));
    AddElement(0, 4, "Stamina+" + std::to_string(m_CurrentItem.first.GetData(1)));
    AddElement(0, 5, "Dexterity+" + std::to_string(m_CurrentItem.first.GetData(2)));
    switch (m_CurrentItem.first.m_SubType)
    {
    case eTypeEquipment::EQUIP_BODY:
    case eTypeEquipment::EQUIP_FOOT:
    case eTypeEquipment::EQUIP_HAND:
    case eTypeEquipment::EQUIP_HEAD:
    case eTypeEquipment::EQUIP_SHOLDER:
        AddElement(0, 6, "Armure+" + std::to_string(m_CurrentItem.first.GetData(3)));
        break;
    case eTypeEquipment::EQUIP_MAIN_HAND:
    case eTypeEquipment::EQUIP_SECOND_HAND:
        AddElement(0, 6, "Attaque+" + std::to_string(m_CurrentItem.first.GetData(3)));
        break;
    default:
        break;
    }
}

void SubMenuEquipment::KeyPress(const sf::Keyboard::Key & p_Key)
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

void SubMenuEquipment::GenericAction(const uint16 & p_ActionID)
{
    g_Socket->SendUnequip(m_CurrentItem.second);
    Close();
}

void SubMenuEquipment::SetCurrentItem(const Item & p_Item, const eTypeEquipment & p_Type)
{
    m_CurrentItem.first = p_Item;
    m_CurrentItem.second = p_Type;
}

MenuEquipment::MenuEquipment() :
    Menu(3, 8)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    m_ElementSize.first = MENU_COLUMN_SIZE * 1.5f;
    m_ElementSize.second = MENU_ROW_SIZE * 2;

    AddElement(0, 0, "-TÊTE");
    AddElement(0, 1, "-EPAULES");
    AddElement(0, 2, "-TORSE");
    AddElement(0, 3, "-MAINS");
    AddElement(0, 4, "-PIEDS");
    AddElement(0, 5, "-ARME1");
    AddElement(0, 6, "-ARME2");
}

MenuEquipment::~MenuEquipment()
{
}

void MenuEquipment::Open()
{
    Menu::Open();
    SetSelectedElement(1, 0);
    SetSubMenu(&m_SubMenu);
}

void MenuEquipment::AddEquipment(const eTypeEquipment & p_Type, const Item & p_Item)
{
    m_Equipment[p_Type] = p_Item;

    uint16 l_Row = GetElementRowSizeAtColumn(0);

    switch (p_Type)
    {
        case eTypeEquipment::EQUIP_HEAD:
        {
            AddElement(1, 0, p_Item.m_Name);
            GetElement(1, 0)->SetFunc(&Menu::GenericAction, 0);
            break;
        }
        case eTypeEquipment::EQUIP_SHOLDER:
        {
            AddElement(1, 1, p_Item.m_Name);
            GetElement(1, 1)->SetFunc(&Menu::GenericAction, 1);
            break;
        }
        case eTypeEquipment::EQUIP_BODY:
        {
            AddElement(1, 2, p_Item.m_Name);
            GetElement(1, 2)->SetFunc(&Menu::GenericAction, 2);
            break;
        }
        case eTypeEquipment::EQUIP_HAND:
        {
            AddElement(1, 3, p_Item.m_Name);
            GetElement(1, 3)->SetFunc(&Menu::GenericAction, 3);
            break;
        }
        case eTypeEquipment::EQUIP_FOOT:
        {
            AddElement(1, 4, p_Item.m_Name);
            GetElement(1, 4)->SetFunc(&Menu::GenericAction, 4);
            break;
        }
    }
}

void MenuEquipment::RemoveEquipment(const eTypeEquipment & p_Type)
{
    if (GetElement(1, p_Type) == nullptr)
        return;

    RemoveElement(1, p_Type);
}

void MenuEquipment::GenericAction(const uint16 & p_Type)
{
    if (m_Equipment.find((eTypeEquipment)p_Type) == m_Equipment.end())
        return;

    m_SubMenu.SetCurrentItem(m_Equipment[(eTypeEquipment)p_Type], (eTypeEquipment)p_Type);
    if (!m_SubMenu.IsOpen())
        m_SubMenu.Open();
}

void MenuEquipment::KeyPress(const sf::Keyboard::Key & p_Key)
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
