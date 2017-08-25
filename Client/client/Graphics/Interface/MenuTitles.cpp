#include "MenuTitles.hpp"
#include "../../Global.hpp"

MenuTitles::MenuTitles() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;

    SetSelectedElement(2, 3);
}

MenuTitles::~MenuTitles()
{
}

void MenuTitles::AddTitle(const uint16 & p_ID, const std::string & p_Name)
{
    m_Titles[p_ID] = p_Name;

    uint16 l_Row = GetElementRowSize();
    AddElement(0, l_Row, p_Name);
    AddElement(1, l_Row, "Appliquer");
    GetElement(1, l_Row)->SetFunc(&Menu::GenericAction, p_ID);
    AddElement(2, l_Row, "Retirer");
    GetElement(2, l_Row)->SetFunc(&Menu::GenericAction2, p_ID);
}

void MenuTitles::RemoveTitle(const uint16 &)
{
}

void MenuTitles::GenericAction(const uint16 & p_MenuID)
{
    g_Socket->SendUpdateTitle(true, p_MenuID);
}

void MenuTitles::GenericAction2(const uint16 & p_MenuID)
{
    g_Socket->SendUpdateTitle(false, p_MenuID);
}

void MenuTitles::KeyPress(const sf::Keyboard::Key & p_Key)
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
