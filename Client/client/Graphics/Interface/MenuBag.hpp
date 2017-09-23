#pragma once
#include "../../Define.hpp"
#include "Menu.hpp"

class SubMenuBag : public Menu
{
public:
    SubMenuBag();
    ~SubMenuBag();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void SetCurrentItem(const Item &, const uint8 &);
    int16 GetItemToDeplace() const;
    void SetItemToDeplace(const int16 &);

private:
    std::pair<Item, uint8> m_CurrentItem;
    int16 m_ItemToDeplace;
};

class MenuBag : public Menu
{
public:
    MenuBag();
    ~MenuBag();
    void Open();
    void KeyPress(const sf::Keyboard::Key &);
    void GenericAction(const uint16 &);
    void AddItem(const uint8 &, const Item &);
    Item* GetItem(const uint8 &);
    void SetStackItem(const uint8 &, const uint8 & );
    void RemoveItem(const uint8 &);
    void SetSize(const uint8 &);
    void SetCurrency(const eTypeCurrency &, const uint16 &);

private:
    std::map<uint8, Item>           m_Items;
    std::map<eTypeCurrency, uint16> m_Currencies;
    uint8                           m_Size;
    SubMenuBag                      m_SubMenu;
};