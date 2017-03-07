#include "Menu.hpp"
#include "../../Global.hpp"

MenuElement::MenuElement()
{
    MenuElement("");
}

MenuElement::MenuElement(const std::string & p_Label) :
    m_Label(p_Label),
    m_Function(nullptr)
{
}

MenuElement::~MenuElement()
{
}

std::string MenuElement::GetLabel() const
{
    return m_Label;
}

void MenuElement::SetFunc(m_Func p_Func, const uint16 & p_Id)
{
    m_Function = p_Func;
    m_IdLabel = p_Id;
}

Menu::Menu() :
    Menu(1, 1)
{
}

Menu::Menu(const uint8 & p_Column, const uint8 & p_Row) :
    m_Column(p_Column),
    m_Row(p_Row),
    m_Open(false)
{
    SetSelectedElement(0, 0);
}

Menu::~Menu()
{
}

void Menu::KeyPress(const eKeyBoardAction & p_Key)
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

bool Menu::IsOpen() const
{
    return m_Open;
}

void Menu::Open()
{
    m_Open = true;
}

void Menu::Close()
{
    m_Open = false;
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