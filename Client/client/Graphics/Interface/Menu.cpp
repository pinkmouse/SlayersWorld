#include "Menu.hpp"
#include "../../Global.hpp"

MenuElement::MenuElement()
{
    MenuElement("");
}

MenuElement::MenuElement(const std::string & p_Label) :
    m_Label(p_Label),
    m_Function(nullptr),
    m_Sprite(nullptr),
    m_Enable(true)
{
}

MenuElement::~MenuElement()
{
}

void MenuElement::SetLabel(const std::string & p_Label)
{
    m_Label = p_Label;
}

std::string MenuElement::GetLabel() const
{
    return m_Label;
}

void MenuElement::Disable()
{
    m_Enable = false;
}

void MenuElement::Enable()
{
    m_Enable = true;
}

bool MenuElement::IsEnable() const
{
    return m_Enable;
}

void MenuElement::SetSprite(sf::Sprite* p_Sprite)
{
    m_Sprite = p_Sprite;
}

sf::Sprite* MenuElement::GetSprite() const
{
    return m_Sprite;
}

void MenuElement::SetFunc(m_Func p_Func, const uint16 & p_Id)
{
    m_Function = p_Func;
    m_IdLabel = p_Id;
}

uint16 MenuElement::GetIDLabel() const
{
    return m_IdLabel;
}

void MenuElement::LaunchFunc(const uint16 & p_Id, Menu * p_Menu)
{
    if (m_Function != nullptr)
        (p_Menu->*(m_Function))(p_Id);
}

bool MenuElement::HasFunc() const
{
    if (m_Function != nullptr)
        return true;
    return false;
}

Menu::Menu() :
    Menu(1, 1)
{
}

Menu::Menu(const uint8 & p_Column, const uint8 & p_Row) :
    m_Column(p_Column),
    m_Row(p_Row),
    m_Open(false),
    m_Title("")
{
    m_ElementSize.first = MENU_COLUMN_SIZE;
    m_ElementSize.second = MENU_ROW_SIZE;
    SetSelectedElement(0, 0);
    m_CursorGraphicBottom = 0;
    m_VisualManager = nullptr;
    m_SubMenu = nullptr;
    m_UselessFieldSelectable = false;
}

Menu::~Menu()
{
}

void Menu::KeyPress(const sf::Keyboard::Key & p_Key)
{
}

std::pair<uint8, uint8> Menu::GetSelectedElement() const
{
    return m_SelectedElement;
}

void Menu::SetSelectedElement(uint8 p_X, uint8 p_Y)
{
    m_SelectedElement.first = p_X;
    m_SelectedElement.second = p_Y;
}

void Menu::SetVisualManager(VisualManager* p_VisualManager)
{
    m_VisualManager = p_VisualManager;
}

Position Menu::GetPosition() const
{
    return m_Pos;
}

uint8 Menu::GetColumn() const
{
    return m_Column;
}

uint8 Menu::GetRow() const
{
    return m_Row;
}

std::pair < uint16, uint16 > Menu::GetElementSize() const
{
    return m_ElementSize;
}


bool Menu::IsOpen() const
{
    return m_Open;
}

void Menu::Open()
{
    m_CursorGraphicBottom = 0;
    m_Open = true;
    SetSelectedElement(0, 0);
}

VisualManager* Menu::GetVisualManager() const
{
    return m_VisualManager;
}

void Menu::Close()
{
    m_Open = false;
}

void Menu::GenericAction(const uint16 & p_MenuID)
{
}

void Menu::GenericAction2(const uint16 & p_MenuID)
{
}


void Menu::SetTitle(const std::string & p_Title)
{
    m_Title = p_Title;
}

std::string Menu::GetTitle() const
{
    return m_Title;
}

std::map<uint8, std::map<uint8, MenuElement> >* Menu::GetElements()
{
    return &m_Elements;
}

MenuElement* Menu::GetElement(const uint8 & p_Col, const uint8 & p_Row)
{
    if (m_Elements.find(p_Col) == m_Elements.end())
        return nullptr;
    if (m_Elements[p_Col].find(p_Row) == m_Elements[p_Col].end())
        return nullptr;
    return &m_Elements[p_Col][p_Row];
}

void Menu::AddElement(const uint8 & p_Col, const uint8 & p_Row, const std::string & p_Label)
{
    m_Elements[p_Col][p_Row] = MenuElement(p_Label);
}

void Menu::RemoveElement(const uint8 & p_Col, const uint8 & p_Row)
{
    if (m_Elements.find(p_Col) == m_Elements.end())
        return;

    std::map<uint8, MenuElement>::iterator l_Itr = m_Elements[p_Col].find(p_Row);
    if (l_Itr == m_Elements[p_Col].end())
        return;

    m_Elements[p_Col].erase(l_Itr);
}

void Menu::ClearElements()
{
    for (std::map<uint8, std::map<uint8, MenuElement> >::iterator l_It = m_Elements.begin(); l_It != m_Elements.end();)
    {
        for (std::map<uint8, MenuElement>::iterator l_Itr = (*l_It).second.begin(); l_Itr != (*l_It).second.end();)
            l_Itr = (*l_It).second.erase(l_Itr);
        l_It = m_Elements.erase(l_It);
    }
}

void Menu::SelectNextElementOn(Orientation p_Orientation)
{
    switch (p_Orientation)
    {
        case Orientation::Up:
        {
            if (m_Elements.find(m_SelectedElement.first) == m_Elements.end())
                break;
            std::map<uint8, MenuElement>::iterator l_It = m_Elements[m_SelectedElement.first].find(m_SelectedElement.second);
            if (l_It == m_Elements[m_SelectedElement.first].end())
                break;

            bool l_HasPass = false;
            while (((!(*l_It).second.HasFunc() || !(*l_It).second.IsEnable()) && !m_UselessFieldSelectable) || !l_HasPass)
            {
                l_It--;
                l_HasPass = true;
                if (l_It == m_Elements[m_SelectedElement.first].end())
                {
                    l_It = m_Elements[m_SelectedElement.first].end();
                    l_It--;
                }
            }

            m_SelectedElement.second = (*l_It).first;
            break;
        }
        case Orientation::Down :
        {
            if (m_Elements.find(m_SelectedElement.first) == m_Elements.end())
                break;
            std::map<uint8, MenuElement>::iterator l_It = m_Elements[m_SelectedElement.first].find(m_SelectedElement.second);
            if (l_It == m_Elements[m_SelectedElement.first].end())
                break;
            bool l_HasPass = false;
            while (((!(*l_It).second.HasFunc() || !(*l_It).second.IsEnable()) && !m_UselessFieldSelectable) || !l_HasPass)
            {
                l_HasPass = true;
                ++l_It;
                if (l_It == m_Elements[m_SelectedElement.first].end())
                    l_It = m_Elements[m_SelectedElement.first].begin();
            }
            m_SelectedElement.second = (*l_It).first;
            break;
        }
        case Orientation::Right:
        {
            std::map<uint8, std::map<uint8, MenuElement> >::iterator l_It = m_Elements.find(m_SelectedElement.first);
            if (l_It == m_Elements.end())
                break;
            std::map<uint8, MenuElement>::iterator l_Itr = m_Elements[m_SelectedElement.first].find(m_SelectedElement.second);
            if (l_Itr == m_Elements[m_SelectedElement.first].end())
                break;
            bool l_HasPass = false;
            while (((!(*l_Itr).second.HasFunc() || !(*l_Itr).second.IsEnable()) && !m_UselessFieldSelectable) || !l_HasPass)
            {
                l_HasPass = true;

                ++l_It;
                if (l_It == m_Elements.end())
                    l_It = m_Elements.begin();

                l_Itr = m_Elements[(*l_It).first].find(m_SelectedElement.second);
                while (l_Itr == m_Elements[(*l_It).first].end())
                {
                    ++l_It;

                    if (l_It == m_Elements.end())
                        l_It = m_Elements.begin();
                    l_Itr = m_Elements[(*l_It).first].find(m_SelectedElement.second);
                }
            }
            m_SelectedElement.first = (*l_It).first;
            break;
        }
        case Orientation::Left:
        {
            std::map<uint8, std::map<uint8, MenuElement> >::iterator l_It = m_Elements.find(m_SelectedElement.first);
            if (l_It == m_Elements.end())
                break;
            std::map<uint8, MenuElement>::iterator l_Itr = m_Elements[m_SelectedElement.first].find(m_SelectedElement.second);
            if (l_Itr == m_Elements[m_SelectedElement.first].end())
                break;
            bool l_HasPass = false;
            while (((!(*l_Itr).second.HasFunc() || !(*l_Itr).second.IsEnable()) && !m_UselessFieldSelectable) || !l_HasPass)
            {
                l_HasPass = true;

                --l_It;
                if (l_It == m_Elements.end())
                {
                    l_It = m_Elements.end();
                    l_It--;
                }
                l_Itr = m_Elements[(*l_It).first].find(m_SelectedElement.second);

                while (l_Itr == m_Elements[(*l_It).first].end())
                {
                    --l_It;

                    if (l_It == m_Elements.end())
                    {
                        l_It = m_Elements.end();
                        l_It--;
                    }
                    l_Itr = m_Elements[(*l_It).first].find(m_SelectedElement.second);
                }
            }

            m_SelectedElement.first = (*l_It).first;
            break;
        }
    }
}

uint8 Menu::GetElementRowSizeAtColumn(const uint8 & p_Col)
{
    if (m_Elements.find(p_Col) == m_Elements.end())
        return 0;

    return m_Elements[p_Col].size();
}

uint8 Menu::GetCursorGraphicBottom() const
{
    return m_CursorGraphicBottom;
}

void Menu::SetCursorGraphicBottom(const uint8 & p_Cursor)
{
    m_CursorGraphicBottom = p_Cursor;
}

Menu* Menu::GetSubMenu()
{
    return m_SubMenu;
}

void Menu::SetSubMenu(Menu* p_Menu)
{
    m_SubMenu = p_Menu;
}

void Menu::CanSelectUselessFields()
{
    m_UselessFieldSelectable = true;
}