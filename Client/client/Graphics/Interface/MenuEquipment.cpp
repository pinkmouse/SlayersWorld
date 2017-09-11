#include "MenuEquipment.hpp"
#include "../../Global.hpp"

MenuEquipment::MenuEquipment() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    AddElement(0, 0, "TITRES");
    AddElement(1, 0, "Retirer");
    GetElement(1, 0)->SetFunc(&Menu::GenericAction2, 0);
}

MenuEquipment::~MenuEquipment()
{
}

void MenuEquipment::Open()
{
    Menu::Open();
    SetSelectedElement(1, 0);
}

void MenuEquipment::AddEquipment(const eTypeEquipment & p_Type, const std::string & p_Name)
{
    m_Equipment[p_Type] = p_Name;

    uint16 l_Row = GetElementRowSizeAtColumn(0);
    AddElement(0, l_Row, p_Name);
    AddElement(1, l_Row, "Appliquer");
    GetElement(1, l_Row)->SetFunc(&Menu::GenericAction, p_Type);
}

void MenuEquipment::RemoveTitle(const uint16 &)
{
}

void MenuEquipment::GenericAction(const uint16 & p_MenuID)
{
    g_Socket->SendUpdateTitle(true, p_MenuID);
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
