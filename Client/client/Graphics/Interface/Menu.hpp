#pragma once
#include "../../Define.hpp"
#include <string>
#include <map>
#include <SFML/Window/Keyboard.hpp>

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
    void LaunchFunc(const uint16 &, Menu *);
    uint16 GetIDLabel() const;

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
    virtual void KeyPress(const sf::Keyboard::Key &);
    MenuElement* GetElement(const uint8 &, const uint8 &);
    std::map<uint8, std::map<uint8, MenuElement> >* GetElements();
    void AddElement(const uint8 &, const uint8 &, const std::string &);
    void RemoveElement(const uint8 &, const uint8 &);
    void ClearElements();
    uint8 GetColumn() const;
    uint8 GetRow() const;
    Position GetPosition() const;
    bool IsOpen() const;
    std::pair<uint8, uint8> GetSelectedElement() const;
    void SetSelectedElement(uint8, uint8);
    void SelectNextElementOn(Orientation);
    void Open();
    void Close();

    virtual void GenericAction(const uint16 &);

protected:
    Position m_Pos;

private:
    bool m_Open;
    uint8 m_Column;
    uint8 m_Row;
    std::pair < uint8, uint8 > m_SelectedElement;
    std::map<uint8, std::map<uint8, MenuElement> > m_Elements;
};