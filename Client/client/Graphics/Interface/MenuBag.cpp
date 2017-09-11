#include "MenuBag.hpp"
#include "../../Global.hpp"

MenuBag::MenuBag() :
    Menu(5, 20)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
}

MenuBag::~MenuBag()
{
}

void MenuBag::Open()
{
    Menu::Open();
    SetSelectedElement(1, 0);
}

void MenuBag::AddItem(const uint8 & p_SlotID, const Item & p_Item)
{
    m_Items[p_SlotID] = p_Item;

    printf("Column = %d Add element on %d -> %d:%d\n", GetColumn(), p_SlotID, p_SlotID % GetColumn(), p_SlotID / GetColumn());
    if (GetElement(p_SlotID % GetColumn(), p_SlotID / GetColumn()) == nullptr)
        AddElement(p_SlotID % GetColumn(), p_SlotID / GetColumn(), p_Item.m_Name + "x" + std::to_string(p_Item.m_StackNb));
}

void MenuBag::RemoveItem(const uint8 &)
{
}

void MenuBag::GenericAction(const uint16 & p_MenuID)
{
    g_Socket->SendUpdateTitle(true, p_MenuID);
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
