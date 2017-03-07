#pragma once
#include "../../Define.hpp"
#include <string>
#include <map>

class Menu;
class MenuElement
{
private:
    using m_Func = void(Menu::*)(const uint16 &);

public:
    MenuElement();
    MenuElement(const std::string &);
    ~MenuElement();
    std::string GetLabel() const;
    void SetFunc(m_Func, const uint16 &);

private:
    std::string     m_Label;
    uint16          m_IdLabel;
    m_Func          m_Function;
};

class Menu
{
public:
    Menu();
    Menu(const uint8 &, const uint8 &);
    ~Menu();
    virtual void KeyPress(const eKeyBoardAction &);
    MenuElement* GetElement(const uint8 &, const uint8 &);
    std::map<uint8, std::map<uint8, MenuElement> >* GetElements();
    void AddElement(const uint8 &, const uint8 &, const std::string &);
    uint8 GetColumn() const;
    uint8 GetRow() const;
    Position GetPosition() const;
    bool IsOpen() const;
    std::pair<uint8, uint8> GetSelectedElement() const;
    void SetSelectedElement(uint8, uint8);
    void Open();
    void Close();

protected:
    Position m_Pos;

private:
    bool m_Open;
    uint8 m_Column;
    uint8 m_Row;
    std::pair < uint8, uint8 > m_SelectedElement;
    std::map<uint8, std::map<uint8, MenuElement> > m_Elements;
};