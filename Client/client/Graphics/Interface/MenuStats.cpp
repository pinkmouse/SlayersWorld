#include "MenuStats.hpp"
#include "../../Global.hpp"

MenuStats::MenuStats() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;

    AddElement(0, 0, "Stats");
    AddElement(0, 2, "Free");
    AddElement(1, 2, "0");
    AddElement(0, 3, "Dexterity");
    AddElement(1, 3, "0");
    AddElement(2, 3, "Add");
    GetElement(2, 3)->SetFunc(&Menu::GenericAction, 0);
    AddElement(3, 3, "Sub");
    GetElement(3, 3)->SetFunc(&Menu::GenericAction, 3);
    AddElement(0, 4, "Force");
    AddElement(1, 4, "0");
    AddElement(2, 4, "Add");
    GetElement(2, 4)->SetFunc(&Menu::GenericAction, 1);
    AddElement(3, 4, "Sub");
    GetElement(3, 4)->SetFunc(&Menu::GenericAction, 4);
    AddElement(0, 5, "Stamina");
    AddElement(1, 5, "0");
    AddElement(2, 5, "Add");
    GetElement(2, 5)->SetFunc(&Menu::GenericAction, 2);
    AddElement(3, 5, "Sub");
    GetElement(3, 5)->SetFunc(&Menu::GenericAction, 5);
    SetSelectedElement(2, 3);
}

MenuStats::~MenuStats()
{
}

void MenuStats::GenericAction(const uint16 & p_MenuID)
{
    switch (p_MenuID)
    {
        case 0:
            g_Socket->SendStatAction(eStats::Dexterity, true, 1);
            break;
        case 3:
            g_Socket->SendStatAction(eStats::Dexterity, false, 1);
            break;
        case 1:
            g_Socket->SendStatAction(eStats::Force, true, 1);
            break;
        case 4:
            g_Socket->SendStatAction(eStats::Force, false, 1);
            break;
        case 2:
            g_Socket->SendStatAction(eStats::Stamina, true, 1);
            break;
        case 5:
            g_Socket->SendStatAction(eStats::Stamina, false, 1);
            break;
    }
}

void MenuStats::KeyPress(const sf::Keyboard::Key & p_Key)
{
    switch (p_Key)
    {
        case sf::Keyboard::Escape:
        {
            if (IsOpen())
                Close();
            break;
        }
        case sf::Keyboard::Return:
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
