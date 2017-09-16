#include "MenuWardrobe.hpp"
#include "../../Global.hpp"

MenuWardrobe::MenuWardrobe() :
    Menu(5, 4)
{
    m_Pos.x = 120;
    m_Pos.y = 30;
    m_ElementSize.first = 100;
    m_ElementSize.second = 100;
}

MenuWardrobe::~MenuWardrobe()
{
}

void MenuWardrobe::Open()
{
    Menu::Open();
    SetSelectedElement(1, 0);
}

void MenuWardrobe::AddSkin(const uint16 & p_ID, const std::string & p_Name)
{
    VisualManager* l_VisualManager = GetVisualManager();
    if (l_VisualManager == nullptr)
        return;
    sf::Sprite* l_SkinSprite = l_VisualManager->GetVisualSprite(eVisualType::VisualSkin, p_ID, 7);

    if (l_SkinSprite == nullptr)
        return;

    m_Skins[p_ID] = p_Name;

    uint16 l_Row = GetElementRowSizeAtColumn(0);
    AddElement(0, l_Row, p_Name);
    GetElement(0, l_Row)->SetSprite(l_SkinSprite);
    AddElement(1, l_Row, "Appliquer");
    GetElement(1, l_Row)->SetFunc(&Menu::GenericAction, p_ID);
}

void MenuWardrobe::RemoveSkin(const uint16 &)
{
}

void MenuWardrobe::GenericAction(const uint16 & p_MenuID)
{
    g_Socket->SendUpdateSkin(p_MenuID);
    Close();
}

void MenuWardrobe::KeyPress(const sf::Keyboard::Key & p_Key)
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
