#pragma once
#include "../../Define.hpp"
#include "../VisualManager.hpp"
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
    bool HasFunc() const;
    uint16 GetIDLabel() const;
    void SetSprite(sf::Sprite*);
    sf::Sprite* GetSprite() const;

private:
    std::string     m_Label;
    uint16          m_IdLabel;
    m_Func          m_Function;
    sf::Sprite*     m_Sprite;
};

class Menu
{
public:
    Menu();
    Menu(const uint8 &, const uint8 &);
    ~Menu();
    void SetTitle(const std::string &);
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
    virtual void Open();
    void Close();
    std::string GetTitle() const;
    VisualManager* GetVisualManager() const;
    uint8 GetElementRowSizeAtColumn(const uint8 &);
    std::pair < uint16, uint16 > GetElementSize() const;
    uint8 GetCursorGraphicBottom() const;
    void SetCursorGraphicBottom(const uint8 &);
    virtual void SetVisualManager(VisualManager*);
    virtual void GenericAction(const uint16 &);
    virtual void GenericAction2(const uint16 &);
    virtual Menu* GetSubMenu();
    virtual void SetSubMenu(Menu*);

protected:
    Position m_Pos;
    std::pair < uint16, uint16 > m_ElementSize;

private:
    bool m_Open;
    uint8 m_Column;
    uint8 m_Row;
    std::string m_Title;
    std::pair < uint8, uint8 > m_SelectedElement;
    uint8 m_CursorGraphicBottom;
    std::map<uint8, std::map<uint8, MenuElement> > m_Elements;
    VisualManager* m_VisualManager;
    Menu* m_SubMenu;
};