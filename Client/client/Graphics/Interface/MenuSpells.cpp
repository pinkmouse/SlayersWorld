#include "MenuSpells.hpp"
#include "../../Global.hpp"

SubMenuSpells::SubMenuSpells() :
    Menu(1, 5)
{
    m_Pos.x = 320;
    m_Pos.y = 130;

    AddElement(0, 0, "Slot #1");
    GetElement(0, 0)->SetFunc(&Menu::GenericAction, 0);
    AddElement(0, 1, "Slot #2");
    GetElement(0, 1)->SetFunc(&Menu::GenericAction, 1);
    AddElement(0, 2, "Slot #3");
    GetElement(0, 2)->SetFunc(&Menu::GenericAction, 2);
    AddElement(0, 3, "Slot #4");
    GetElement(0, 3)->SetFunc(&Menu::GenericAction, 3);
    AddElement(0, 4, "Slot #5");
    GetElement(0, 4)->SetFunc(&Menu::GenericAction, 4);
}

SubMenuSpells::~SubMenuSpells()
{
}

void SubMenuSpells::Open()
{
    Menu::Open();
    SetSelectedElement(0, 0);
}

void SubMenuSpells::KeyPress(const sf::Keyboard::Key & p_Key)
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

void SubMenuSpells::GenericAction(const uint16 & p_ActionID)
{
    printf("--> Action on %d\n", p_ActionID);
    Close();
}

void SubMenuSpells::SetCurrentSpell(const uint16 & p_SpellID)
{
    m_CurrentSpell = p_SpellID;
}


MenuSpells::MenuSpells() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    SetSubMenu(&m_SubMenu);
}

MenuSpells::~MenuSpells()
{
}

void MenuSpells::Open()
{
    Menu::Open();
    SetSelectedElement(3, 0);
}

void MenuSpells::AddSpell(const uint16 & p_ID, const std::string & p_Name)
{
    m_Spells[p_ID] = p_Name;

    uint16 l_Row = GetElementRowSizeAtColumn(0);
    AddElement(0, l_Row, p_Name);
    AddElement(3, l_Row, "Appliquer");
    GetElement(3, l_Row)->SetFunc(&Menu::GenericAction, p_ID);
}

void MenuSpells::RemoveTitle(const uint16 &)
{
}

void MenuSpells::GenericAction(const uint16 & p_MenuID)
{
    if (!m_SubMenu.IsOpen())
        m_SubMenu.Open();

    m_SubMenu.SetCurrentSpell(p_MenuID);
}

void MenuSpells::KeyPress(const sf::Keyboard::Key & p_Key)
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
